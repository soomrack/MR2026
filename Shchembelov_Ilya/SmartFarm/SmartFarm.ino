#include "Temp_Humid.h"
//#include "Light.h"

AHT21 TH_sensor;

void setup() {
  TH_sensor.INIT();
  Serial.begin(9600);

}

void loop() {
  Serial.println(TH_sensor.get_humidity());

}
