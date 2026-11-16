#include "Temp_Humid.h"     //Class for air temperature and humidity sensor
#include "Light.h"          //Class for light sensor
#include "SoilMoisture.h"   //Class for soil moisture sensor
#include "Pump.h"           //Class for water pump
#include "Fan.h"            //Class for ventilation fan
#include "Heater.h"         //Class for heater
#include "GrowLight.h"      //Class for grow light
#include "Greenhouse.h"     //Class for boundary conditions of all scenarios

AHT21 TH_sensor;            //Creating sensors objects
LightSensor Light_S(A0);
SoilSensor soil(A1);

Pump water_pump(2);         //Creating actuators objects
Fan fan(3);
Heater heater(4);
GrowLight grow_light(5);

Greenhouse greenhouse;      //Creating object with boundary conditions for all scenarios

void setup() {
  TH_sensor.INIT();         //Initialization for temperature and humidity sensor
  Serial.begin(9600);       //Ititializing serial monitor and setting up speed to 9600 bot

}

void loop() {
  float temperature = TH_sensor.get_temperature();    //Reading air temperature
  float humidity = TH_sensor.get_humidity();          //Reading air humidity

  #ifdef DEBUG
    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.print(" Humidity: ");
    Serial.println(humidity);
  #endif

  control_heater(temperature, humidity);              //Heater control scenario
  control_fan(temperature, humidity);                 //Fan control scenario
  control_irrigation();                               //Irrigation scenario
  control_lighting();                                 //Lighting scenario

}


// Heater control scenario:
// high temperature or low humidity turns the heater off
// low temperature or high humidity turns the heater on

void control_heater(float temperature, float humidity) {
  if (temperature >= greenhouse.get_temp_high()) {
    heater.off();
  } else if (temperature <= greenhouse.get_temp_low()) {
    heater.on();
  } else if (humidity >= greenhouse.get_humid_high()) {
    heater.on();
  } else if (humidity <= greenhouse.get_humid_low()) {
    heater.off();
  }
}


// Fan control scenario:
// high temperature or high humidity turns the fan on
// low humidity turns the fan off

void control_fan(float temperature, float humidity) {
  if (temperature >= greenhouse.get_temp_high()) {
    fan.on();
  } else if (humidity >= greenhouse.get_humid_high()) {
    fan.on();
  } else if (humidity <= greenhouse.get_humid_low()) {
    fan.off();
  }
}


// Irrigation scenario:
// dry soil turns the pump on for watering time
// after watering the soil rests for soaking time before the next check
// normal or wet soil keeps the pump off

void control_irrigation() {
  const uint8_t CHECK_SOIL = 0;         // State for checking soil moisture
  const uint8_t WATERING = 1;           // State for watering
  const uint8_t SOAKING = 2;            // State for waiting for water to soak in

  static uint8_t state = CHECK_SOIL;    // Current state of the irrigation scenario
  static uint32_t timer = 0;            // State start time in milliseconds

  switch (state) {
    case CHECK_SOIL:
      if (soil.get_moisture_level() <= greenhouse.get_soil_dry_level()) {
        water_pump.on();
        timer = millis();
        state = WATERING;
      }
      break;
    case WATERING:
      if (millis() - timer >= greenhouse.get_watering_time()) {
        water_pump.off();
        timer = millis();
        state = SOAKING;
      }
      break;
    case SOAKING:
      if (millis() - timer >= greenhouse.get_soaking_time()) {
        state = CHECK_SOIL;
      }
      break;
  }
}


// Lighting scenario:
// darkness turns the grow light on
// bright light turns the grow light off

void control_lighting() {
  float light_level = Light_S.get_lumen_value();
  if (light_level <= greenhouse.get_light_on_level()) {
    grow_light.on();
  } else if (light_level >= greenhouse.get_light_off_level()) {
    grow_light.off();
  }
}
