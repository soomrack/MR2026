#include <DHT.h>

// пины
const uint8_t PIN_DHT    = 2;
const uint8_t PIN_SOIL   = A0;
const uint8_t PIN_LIGHT  = A1;
const uint8_t PIN_FAN    = 5;
const uint8_t PIN_HEATER = 6;
const uint8_t PIN_LAMP   = 7;
const uint8_t PIN_PUMP   = 8;

const bool RELAY_ACTIVE_LOW = true;

// калибровка
const int SOIL_RAW_DRY     = 600;
const int SOIL_RAW_WET     = 280;
const int LIGHT_RAW_DARK   = 900;
const int LIGHT_RAW_BRIGHT = 100;

// климат для томатов
const float TEMP_MIN     = 18.0;
const float TEMP_MAX     = 26.0;
const float HUMIDITY_MAX = 70.0;
const float SOIL_MIN     = 40.0;
const float LIGHT_MIN    = 40.0;

// гистерезис и времена
const float TEMP_HYST  = 1.0;
const float HUM_HYST   = 5.0;
const float LIGHT_HYST = 10.0;
const unsigned long CONTROL_PERIOD_MS = 2000;
const unsigned long LAMP_MIN_ON_MS    = 10UL * 60 * 1000;
const unsigned long PUMP_ON_MS        = 4000;
const unsigned long PUMP_SOAK_MS      = 60000;

// ---------- датчики ----------
class Sensor {
public:
  explicit Sensor(const char* name) : name_(name) {}
  virtual void begin() {}
  void update() { value_ = measure(); }
  float value() const { return value_; }
  bool  isOk()  const { return !isnan(value_); }
  const char* name() const { return name_; }
protected:
  virtual float measure() = 0;
private:
  const char* name_;
  float value_ = NAN;
};

class DhtTemperature : public Sensor {
public:
  DhtTemperature(const char* name, DHT& dht) : Sensor(name), dht_(dht) {}
  void begin() override { dht_.begin(); }
protected:
  float measure() override { return dht_.readTemperature(); }
private:
  DHT& dht_;
};

class DhtHumidity : public Sensor {
public:
  DhtHumidity(const char* name, DHT& dht) : Sensor(name), dht_(dht) {}
  void begin() override { dht_.begin(); }
protected:
  float measure() override { return dht_.readHumidity(); }
private:
  DHT& dht_;
};

class AnalogPercentSensor : public Sensor {
public:
  AnalogPercentSensor(const char* name, uint8_t pin, int rawAt0, int rawAt100)
    : Sensor(name), pin_(pin), raw0_(rawAt0), raw100_(rawAt100) {}
protected:
  float measure() override {
    float percent = (analogRead(pin_) - raw0_) * 100.0 / (raw100_ - raw0_);
    return constrain(percent, 0.0, 100.0);
  }
private:
  uint8_t pin_;
  int raw0_;
  int raw100_;
};

// ---------- актуаторы ----------
// включается, если за цикл была хотя бы одна заявка request()
class Actuator {
public:
  explicit Actuator(const char* name) : name_(name) {}
  virtual void begin() {}
  void request() { requested_ = true; }
  void apply() {
    if (requested_ != on_) {
      on_ = requested_;
      write(on_);
    }
    requested_ = false;
  }
  bool isOn() const { return on_; }
  const char* name() const { return name_; }
protected:
  virtual void write(bool on) = 0;
private:
  const char* name_;
  bool on_ = false;
  bool requested_ = false;
};

class RelayActuator : public Actuator {
public:
  RelayActuator(const char* name, uint8_t pin, bool activeLow = RELAY_ACTIVE_LOW)
    : Actuator(name), pin_(pin), activeLow_(activeLow) {}
  void begin() override {
    write(false);
    pinMode(pin_, OUTPUT);
  }
protected:
  void write(bool on) override {
    digitalWrite(pin_, (on != activeLow_) ? HIGH : LOW);
  }
private:
  uint8_t pin_;
  bool activeLow_;
};

// ---------- конфигурация теплицы ----------
DHT dht(PIN_DHT, DHT22);

DhtTemperature      airTemp     ("airT",  dht);
DhtHumidity         airHumidity ("airH",  dht);
AnalogPercentSensor soilMoisture("soil",  PIN_SOIL,  SOIL_RAW_DRY,   SOIL_RAW_WET);
AnalogPercentSensor light       ("light", PIN_LIGHT, LIGHT_RAW_DARK, LIGHT_RAW_BRIGHT);

RelayActuator fan   ("fan",    PIN_FAN);
RelayActuator heater("heater", PIN_HEATER);
RelayActuator lamp  ("lamp",   PIN_LAMP);
RelayActuator pump  ("pump",   PIN_PUMP);

Sensor*   sensors[]   = { &airTemp, &airHumidity, &soilMoisture, &light };
Actuator* actuators[] = { &fan, &heater, &lamp, &pump };

// ---------- регулирование ----------
enum PumpState { PUMP_IDLE, PUMP_POURING, PUMP_SOAKING };

void regulate() {
  static bool tooHot = false, tooCold = false, tooHumid = false;
  static bool lampWanted = false;
  static unsigned long lampOnSince = 0;
  static PumpState pumpState = PUMP_IDLE;
  static unsigned long pumpTimer = 0;
  unsigned long now = millis();

  // температура
  if (airTemp.isOk()) {
    float t = airTemp.value();
    if (t > TEMP_MAX)                  tooHot = true;
    else if (t < TEMP_MAX - TEMP_HYST) tooHot = false;
    if (t < TEMP_MIN)                  tooCold = true;
    else if (t > TEMP_MIN + TEMP_HYST) tooCold = false;
  } else {
    tooHot = tooCold = false;
  }

  // влажность
  if (!airHumidity.isOk())                                tooHumid = false;
  else if (airHumidity.value() > HUMIDITY_MAX)            tooHumid = true;
  else if (airHumidity.value() < HUMIDITY_MAX - HUM_HYST) tooHumid = false;

  // свет
  if (!lampWanted && light.value() < LIGHT_MIN) {
    lampWanted = true;
    lampOnSince = now;
  } else if (lampWanted && light.value() > LIGHT_MIN + LIGHT_HYST &&
             now - lampOnSince >= LAMP_MIN_ON_MS) {
    lampWanted = false;
  }

  // полив порциями
  switch (pumpState) {
    case PUMP_IDLE:
      if (soilMoisture.value() < SOIL_MIN) { pumpState = PUMP_POURING; pumpTimer = now; }
      break;
    case PUMP_POURING:
      if (now - pumpTimer >= PUMP_ON_MS) { pumpState = PUMP_SOAKING; pumpTimer = now; }
      break;
    case PUMP_SOAKING:
      if (now - pumpTimer >= PUMP_SOAK_MS) pumpState = PUMP_IDLE;
      break;
  }

  if (tooHot || tooCold || tooHumid) fan.request();
  if (tooCold)                       heater.request();
  if (lampWanted)                    lamp.request();
  if (pumpState == PUMP_POURING)     pump.request();
}

bool controlStepDue() {
  static unsigned long last = 0;
  if (millis() - last < CONTROL_PERIOD_MS) return false;
  last = millis();
  return true;
}

void printStatus() {
  for (Sensor* s : sensors) {
    Serial.print(s->name());
    Serial.print('=');
    if (s->isOk()) Serial.print(s->value(), 1);
    else           Serial.print(F("ERR"));
    Serial.print(' ');
  }
  Serial.print('|');
  for (Actuator* a : actuators) {
    Serial.print(' ');
    Serial.print(a->name());
    Serial.print('=');
    Serial.print(a->isOn() ? "ON" : "off");
  }
  Serial.println();
}

void setup() {
  Serial.begin(9600);
  for (Sensor* s : sensors)     s->begin();
  for (Actuator* a : actuators) a->begin();
  Serial.println(F("Step 3: sensors and actuators as objects"));
}

void loop() {
  if (!controlStepDue()) return;

  for (Sensor* s : sensors)     s->update();
  regulate();
  for (Actuator* a : actuators) a->apply();

  printStatus();
}
