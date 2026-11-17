// Умная теплица — шаг 4: алгоритмы регулирования как отдельные объекты.
#include <DHT.h>

// ---------- Подключение и калибровка ----------
const uint8_t PIN_HEATER = 3;
const uint8_t PIN_PUMP   = 5;
const uint8_t PIN_LAMP   = 6;   // светодиодная лента
const uint8_t PIN_FAN    = 7;
const uint8_t PIN_DHT    = 8;
const uint8_t PIN_SOIL   = A0;
const uint8_t PIN_LIGHT  = A1;

const bool RELAY_ACTIVE_LOW = true;  // false, если нагрузка включается уровнем HIGH

const int SOIL_RAW_DRY     = 600;
const int SOIL_RAW_WET     = 280;
const int LIGHT_RAW_DARK   = 900;
const int LIGHT_RAW_BRIGHT = 100;

const float TEMP_HYST  = 1.0;    // °C
const float HUM_HYST   = 5.0;    // %
const float LIGHT_HYST = 10.0;   // %
const unsigned long CONTROL_PERIOD_MS = 2000;
const unsigned long LAMP_MIN_ON_MS    = 10UL * 60 * 1000;
const unsigned long PUMP_ON_MS        = 4000;
const unsigned long PUMP_SOAK_MS      = 60000;

// ---------- Датчики ----------
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

// ---------- Актуаторы ----------
// Регуляторы подают заявки request(), apply() включает, если заявка была.
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

// ---------- Климат ----------
struct ClimateProfile {
  const char* name;
  float tempMin;
  float tempMax;
  float humidityMax;
  float soilMin;
  float lightMin;
};

const ClimateProfile TOMATO = { "Tomato", 18, 26, 70, 40, 40 };

// ---------- Регуляторы ----------
class Regulator {
public:
  virtual void update(const ClimateProfile& c) = 0;
};

// Двухпозиционное регулирование: между порогами сохраняется прежнее состояние
class Hysteresis {
public:
  bool above(float value, float onLevel, float offLevel) {
    if (value > onLevel)       state_ = true;
    else if (value < offLevel) state_ = false;
    return state_;
  }
  bool below(float value, float onLevel, float offLevel) {
    if (value < onLevel)       state_ = true;
    else if (value > offLevel) state_ = false;
    return state_;
  }
private:
  bool state_ = false;
};

class Lighting : public Regulator {
public:
  Lighting(Sensor& light, Actuator& lamp) : light_(light), lamp_(lamp) {}
  void update(const ClimateProfile& c) override {
    if (light_.isOk()) {
      float v = light_.value();
      if (!on_ && v < c.lightMin) {
        on_ = true;
        onSince_ = millis();
      } else if (on_ && v > c.lightMin + LIGHT_HYST &&
                 millis() - onSince_ >= LAMP_MIN_ON_MS) {
        on_ = false;
      }
    }
    if (on_) lamp_.request();
  }
private:
  Sensor& light_;
  Actuator& lamp_;
  bool on_ = false;
  unsigned long onSince_ = 0;
};

// Полив порциями с паузой на впитывание
class Watering : public Regulator {
public:
  Watering(Sensor& soil, Actuator& pump) : soil_(soil), pump_(pump) {}
  void update(const ClimateProfile& c) override {
    unsigned long now = millis();
    switch (state_) {
      case IDLE:
        if (soil_.isOk() && soil_.value() < c.soilMin) { state_ = POURING; since_ = now; }
        break;
      case POURING:
        if (now - since_ >= PUMP_ON_MS) { state_ = SOAKING; since_ = now; }
        break;
      case SOAKING:
        if (now - since_ >= PUMP_SOAK_MS) state_ = IDLE;
        break;
    }
    if (state_ == POURING) pump_.request();
  }
private:
  enum State { IDLE, POURING, SOAKING };
  Sensor& soil_;
  Actuator& pump_;
  State state_ = IDLE;
  unsigned long since_ = 0;
};

class Cooling : public Regulator {
public:
  Cooling(Sensor& temp, Actuator& fan) : temp_(temp), fan_(fan) {}
  void update(const ClimateProfile& c) override {
    if (temp_.isOk() && hot_.above(temp_.value(), c.tempMax, c.tempMax - TEMP_HYST))
      fan_.request();
  }
private:
  Sensor& temp_;
  Actuator& fan_;
  Hysteresis hot_;
};

class Heating : public Regulator {
public:
  Heating(Sensor& temp, Actuator& heater, Actuator& fan)
    : temp_(temp), heater_(heater), fan_(fan) {}
  void update(const ClimateProfile& c) override {
    if (temp_.isOk() && cold_.below(temp_.value(), c.tempMin, c.tempMin + TEMP_HYST)) {
      heater_.request();
      fan_.request();
    }
  }
private:
  Sensor& temp_;
  Actuator& heater_;
  Actuator& fan_;
  Hysteresis cold_;
};

class Dehumidification : public Regulator {
public:
  Dehumidification(Sensor& humidity, Actuator& fan) : humidity_(humidity), fan_(fan) {}
  void update(const ClimateProfile& c) override {
    if (humidity_.isOk() &&
        humid_.above(humidity_.value(), c.humidityMax, c.humidityMax - HUM_HYST))
      fan_.request();
  }
private:
  Sensor& humidity_;
  Actuator& fan_;
  Hysteresis humid_;
};

// ---------- Конфигурация теплицы ----------
DHT dht(PIN_DHT, DHT22);

DhtTemperature      airTemp     ("airT",  dht);
DhtHumidity         airHumidity ("airH",  dht);
AnalogPercentSensor soilMoisture("soil",  PIN_SOIL,  SOIL_RAW_DRY,   SOIL_RAW_WET);
AnalogPercentSensor light       ("light", PIN_LIGHT, LIGHT_RAW_DARK, LIGHT_RAW_BRIGHT);

RelayActuator fan   ("fan",    PIN_FAN);
RelayActuator heater("heater", PIN_HEATER);
RelayActuator lamp  ("lamp",   PIN_LAMP);
RelayActuator pump  ("pump",   PIN_PUMP);

Lighting         lighting     (light,        lamp);           // правило 1
Watering         watering     (soilMoisture, pump);           // правило 2
Cooling          cooling      (airTemp,      fan);            // правило 3
Heating          heating      (airTemp,      heater, fan);    // правило 4
Dehumidification dehumidifying(airHumidity,  fan);            // правило 5

Sensor*    sensors[]    = { &airTemp, &airHumidity, &soilMoisture, &light };
Actuator*  actuators[]  = { &fan, &heater, &lamp, &pump };
Regulator* regulators[] = { &lighting, &watering, &cooling, &heating, &dehumidifying };

const ClimateProfile& climate = TOMATO;

// ---------- Служебное ----------
bool controlStepDue() {
  static unsigned long last = 0;
  if (millis() - last < CONTROL_PERIOD_MS) return false;
  last = millis();
  return true;
}

void printStatus() {
  Serial.print('[');
  Serial.print(climate.name);
  Serial.print(F("] "));
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

// ---------- Основная программа ----------
void setup() {
  Serial.begin(9600);
  for (Sensor* s : sensors)     s->begin();
  for (Actuator* a : actuators) a->begin();
  Serial.println(F("Step 4: regulators as objects"));
}

void loop() {
  if (!controlStepDue()) return;

  for (Sensor* s : sensors)       s->update();         // измерить
  for (Regulator* r : regulators) r->update(climate);  // решить
  for (Actuator* a : actuators)   a->apply();          // исполнить

  printStatus();
}
