#include <Wire.h>                                               // Library for interaction with i2c interface
#include <AHTxx.h>                                              // Library for interaction with AHTxx sensors

class AHT21 : public AHTxx {                                    // Class for procesing sensor value
private:
  float _ahtval;                                                // Attribute for sensor value (temperature or humidity)
  uint8_t _err_code;                                            // Error code
  AHTxx _sensor;                                                // Sensor object from parent library
  uint8_t _err_get();                                           // Returns error code

public:
  AHT21() : _sensor(AHTXX_ADDRESS_X38, AHT2x_SENSOR) {}         // Creates object and configures sensors pin to input
  void INIT();                                                  // Initialization method
  float get_temperature();                                      // Returns curent temperature
  float get_humidity();                                         // Returns curent humidity
  void err_decode();                                            // Prints error discription
};



void AHT21::INIT() {
  Wire.begin();

  // Запуск и проверка датчика
  if (_sensor.begin() == false) {
    #ifdef DEBUG
        Serial.println("AHT21 initialization failed!");
    #endif
  } else {
    #ifdef DEBUG
        Serial.println("AHT21 initialized successfully.");
    #endif
  }
}

float AHT21::get_temperature() {
  _ahtval = _sensor.readTemperature();
  if (_ahtval != AHTXX_ERROR) {
    return _ahtval;
  } else {
    _err_code = _err_get();
    if (_sensor.softReset() == true) return get_temperature();
    else return -300;
  }
}

float AHT21::get_humidity() {
  _ahtval = _sensor.readHumidity();
  if (_ahtval != AHTXX_ERROR) {
    return _ahtval;
  } else {
    _err_code = _err_get();
    if (_sensor.softReset() == true) return get_humidity();
    else return -1;
  }
}

uint8_t AHT21::_err_get() {
  return _sensor.getStatus();
}


void AHT21::err_decode() {
  switch (_err_code) {
    case AHTXX_NO_ERROR:
      Serial.println(F("no error"));
      break;

    case AHTXX_BUSY_ERROR:
      Serial.println(F("sensor busy, increase polling time"));
      break;

    case AHTXX_ACK_ERROR:
      Serial.println(F("sensor didn't return ACK, not connected, broken, long wires (reduce speed), bus locked by slave (increase stretch limit)"));
      break;

    case AHTXX_DATA_ERROR:
      Serial.println(F("received data smaller than expected, not connected, broken, long wires (reduce speed), bus locked by slave (increase stretch limit)"));
      break;

    case AHTXX_CRC8_ERROR:
      Serial.println(F("computed CRC8 not match received CRC8, this feature supported only by AHT2x sensors"));
      break;

    default:
      Serial.println(F("unknown status"));
      break;
  }
}
