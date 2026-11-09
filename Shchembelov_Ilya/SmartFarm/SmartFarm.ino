#include "Temp_Humid.h"     //Class for air temperature and humidity sensor
#include "Light.h"          //Class for light sensor
#include "SoilMoisture.h"   //Class for soil moisture sensor

AHT21 TH_sensor;            //Creating sensors objects
LightSensor Light_S(A0);
SoilSensor soil(A1);

void setup() {
  TH_sensor.INIT();         //Initialization for temperature and humidity sensor
  Serial.begin(9600);       //Ititializing serial monitor and setting up speed to 9600 bot

}

void loop() {
  Serial.println(Light_S.get_lumen_value());

}
