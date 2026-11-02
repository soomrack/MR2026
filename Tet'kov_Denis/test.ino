/*
 * Умная теплица — Шаг 1. Проверка оборудования.
 *
 * Цель шага: убедиться, что все датчики и реле подключены правильно,
 * и снять "сырые" значения аналоговых датчиков для калибровки.
 *
 * Подключение (Arduino UNO):
 *   DHT22 (температура и влажность воздуха) -> D2
 *   Датчик влажности почвы (аналоговый)     -> A0
 *   Фоторезистор (освещённость)             -> A1
 *   Реле вентилятора                        -> D5
 *   Реле нагревателя                        -> D6
 *   Реле лампы                              -> D7
 *   Реле помпы                              -> D8
 *
 * Библиотеки: "DHT sensor library" (Adafruit) и "Adafruit Unified Sensor".
 *
 * Команды в мониторе порта (9600 бод):
 *   1 - вентилятор вкл/выкл
 *   2 - нагреватель вкл/выкл
 *   3 - лампа вкл/выкл
 *   4 - помпа вкл/выкл
 *   0 - всё выключить
 *
 * Калибровка: запишите soilRaw для сухой почвы и для стакана воды,
 * lightRaw для закрытого рукой датчика и для яркого света.
 * Эти числа понадобятся на шаге 2.
 */
#include <DHT.h>

const uint8_t PIN_DHT    = 2;
const uint8_t PIN_SOIL   = A0;
const uint8_t PIN_LIGHT  = A1;
const uint8_t PIN_FAN    = 5;
const uint8_t PIN_HEATER = 6;
const uint8_t PIN_LAMP   = 7;
const uint8_t PIN_PUMP   = 8;

// Большинство модулей реле включаются НИЗКИМ уровнем (active LOW).
// Если ваше реле включается высоким уровнем — поставьте false.
const bool RELAY_ACTIVE_LOW = true;

DHT dht(PIN_DHT, DHT22);

bool fanOn    = false;
bool heaterOn = false;
bool lampOn   = false;
bool pumpOn   = false;

// Включить/выключить реле с учётом его логики (active LOW / HIGH)
void setRelay(uint8_t pin, bool on) {
  bool level = RELAY_ACTIVE_LOW ? !on : on;
  digitalWrite(pin, level ? HIGH : LOW);
}

// Сначала задаём уровень "выключено", потом делаем пин выходом —
// так реле не щёлкнет при включении платы.
void initRelay(uint8_t pin) {
  setRelay(pin, false);
  pinMode(pin, OUTPUT);
}

void updateRelays() {
  setRelay(PIN_FAN, fanOn);
  setRelay(PIN_HEATER, heaterOn);
  setRelay(PIN_LAMP, lampOn);
  setRelay(PIN_PUMP, pumpOn);
}

void handleCommand() {
  if (!Serial.available()) return;
  char c = Serial.read();
  switch (c) {
    case '1': fanOn    = !fanOn;    break;
    case '2': heaterOn = !heaterOn; break;
    case '3': lampOn   = !lampOn;   break;
    case '4': pumpOn   = !pumpOn;   break;
    case '0': fanOn = heaterOn = lampOn = pumpOn = false; break;
    default:  return;   // игнорируем \r, \n и прочие символы
  }
  updateRelays();
}

void printValue(const __FlashStringHelper* label, float value) {
  Serial.print(label);
  if (isnan(value)) Serial.print(F("ERR"));   // DHT не ответил
  else              Serial.print(value, 1);
}

void setup() {
  Serial.begin(9600);
  dht.begin();
  initRelay(PIN_FAN);
  initRelay(PIN_HEATER);
  initRelay(PIN_LAMP);
  initRelay(PIN_PUMP);
  Serial.println(F("Step 1: hardware test"));
  Serial.println(F("Commands: 1-fan 2-heater 3-lamp 4-pump 0-all off"));
}

void loop() {
  handleCommand();

  // Печатаем показания раз в 2 секунды (DHT22 чаще не опрашивают)
  static unsigned long lastPrint = 0;
  if (millis() - lastPrint < 2000) return;
  lastPrint = millis();

  float airT     = dht.readTemperature();
  float airH     = dht.readHumidity();
  int   soilRaw  = analogRead(PIN_SOIL);
  int   lightRaw = analogRead(PIN_LIGHT);

  printValue(F("airT="), airT);
  printValue(F(" airH="), airH);
  Serial.print(F(" soilRaw="));  Serial.print(soilRaw);
  Serial.print(F(" lightRaw=")); Serial.print(lightRaw);
  Serial.print(F(" | fan="));    Serial.print(fanOn);
  Serial.print(F(" heater="));   Serial.print(heaterOn);
  Serial.print(F(" lamp="));     Serial.print(lampOn);
  Serial.print(F(" pump="));     Serial.print(pumpOn);
  Serial.println();
}
