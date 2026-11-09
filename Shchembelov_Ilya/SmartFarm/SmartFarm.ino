#include "Temp_Humid.h"
#include "Light.h"
#include "SoilMoisture.h"

AHT21 TH_sensor;
LightSensor Light_S(A0);
SoilSensor soil(A1);

void setup() {
  TH_sensor.INIT();
  Serial.begin(9600);

}

void loop() {
  Serial.println(Light_S.get_lumen_value());

}
