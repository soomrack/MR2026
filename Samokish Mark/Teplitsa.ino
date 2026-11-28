#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

#define DHTPIN 2 //подключение DHT 11 к цифровому 2
#define DHTTYPE    DHT11

DHT_Unified dht(DHTPIN, DHTTYPE);

int MSdelay;

void setup() {

   Serial.begin(9600);
  // Initialize device.
  dht.begin();
  // put your setup code here, to run once:
// - Датчики:
//  1. температура воздуха - конкретный датчик
  //2. влажность воздуха - конкретный датчик - 2 в 1 DHT11
  
   Serial.println("DHT 11. Humidity sensor. Initialistaion test");
     sensor_t sensor;
     
     dht.temperature().getSensor(&sensor);
  Serial.println("------------------------------------");
  Serial.println("Temperature Sensor DHT11");
  Serial.print  ("Sensor Type: "); Serial.println(sensor.name);
  Serial.print  ("Driver Ver:  "); Serial.println(sensor.version);
  Serial.print  ("Unique ID:   "); Serial.println(sensor.sensor_id);
  Serial.print  ("Max Value:   "); Serial.print(sensor.max_value); Serial.println(F("°C"));
  Serial.print  ("Min Value:   "); Serial.print(sensor.min_value); Serial.println(F("°C"));
  Serial.print  ("Resolution:  "); Serial.print(sensor.resolution); Serial.println(F("°C"));
  Serial.println(F("------------------------------------"));


     dht.humidity().getSensor(&sensor);
  Serial.println(F("Humidity Sensor DHT11"));
  Serial.print  (F("Sensor Type: ")); Serial.println(sensor.name);
  Serial.print  (F("Driver Ver:  ")); Serial.println(sensor.version);
  Serial.print  (F("Unique ID:   ")); Serial.println(sensor.sensor_id);
  Serial.print  (F("Max Value:   ")); Serial.print(sensor.max_value); Serial.println(F("%"));
  Serial.print  (F("Min Value:   ")); Serial.print(sensor.min_value); Serial.println(F("%"));
  Serial.print  (F("Resolution:  ")); Serial.print(sensor.resolution); Serial.println(F("%"));
  Serial.println(F("------------------------------------"));
  // Set delay between sensor readings based on sensor details.
  delayMS = sensor.min_delay / 1000;







}

     
   
  //3. влажность почвы - опять
 // 4. освещенность - фоторезистор лол
//- Актуаторы:
  //1. вентилятор - мотор и вероятно контроллер мотора, скорее всего PWM
  //2. нагреватель - ну... медная труба-короткое замыкание?
  //3. лампа - светодиод
  //4. помпа - просто модуль ведь, да? 
  
  // старт - снятие занчений со всех датчиков - проверка на адеватность, какой-то способ визуальнтго подтверждения значений.
  Serial.begin(9600);
  
  // 
 }

void loop() {
// put your main code here, to run repeatedly:
 // Delay between measurements.
  delay(delayMS);
  // Get temperature event and print its value.
  sensors_event_t event;
  dht.temperature().getEvent(&event);
  if (isnan(event.temperature)) {
    Serial.println(F("Error reading temperature!"));
  }
  else {
    Serial.print(F("Temperature: "));
    Serial.print(event.temperature);
    Serial.println(F("°C"));
  }
  // Get humidity event and print its value.
  dht.humidity().getEvent(&event);
  if (isnan(event.relative_humidity)) {
    Serial.println(F("Error reading humidity!"));
  }
  else {
    Serial.print(F("Humidity: "));
    Serial.print(event.relative_humidity);
    Serial.println(F("%"));
  }
// в зависимости от того или иного ввода меняется значение актуатора.
}
