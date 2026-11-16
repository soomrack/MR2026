class SoilSensor {                                                // Creating class for soil moisture sensor
private:
  uint8_t _pin;                                                   // Attribute for pin number of the sensor
  uint16_t _m_value;                                              // Last raw value from the sensor
  uint16_t _air_soil = 400;                                       // Border between air and dry soil
  uint16_t _wet_dry = 200;                                        // Border between wet and dry soil
public:
  SoilSensor(uint8_t pin)                                         // Creates object and configures sensors pin to input
    : _pin(pin) {
    pinMode(_pin, INPUT);
  }
  void set_air_soil_boarder(uint16_t value);                      // Method to set up border between air and dry soil
  void set_wet_dry_border(uint16_t value);                        // Method to set up border between wet and dry soil
  uint16_t get_raw_moisture();                                    // Method returns raw value from soil moisture sensor
  uint8_t get_moisture_level();                                   // Method returns soil moisture level
};

void SoilSensor::set_air_soil_boarder(uint16_t value) {
  _air_soil = value;
#ifdef DEBUG
  Serial.print("New value for boarder betwen air and dry soil is set to ");
  Serila.println(_air_soil);
#endif
}

void SoilSensor::set_wet_dry_border(uint16_t value) {
  _wet_dry = value;
#ifdef DEBUG
  Serial.print("New value for boarder betwen wet and dry soil is set to ");
  Serila.println(_air_soil);
#endif
}

uint16_t SoilSensor::get_raw_moisture() {
  _m_value = analogRead(_pin);
#ifdef DEBUG
  Serial.print("Raw moisture value ");
  Serila.println(_m_value);
#endif
  return analogRead(_pin);
}

uint8_t SoilSensor::get_moisture_level(){
  get_raw_moisture();
  if (_m_value < _air_soil) return 0;
  else if (_m_value < _wet_dry) return 1;
  else return 2;
}
