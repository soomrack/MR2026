class LightSensor {                                                 // Creating class for light sensor
private:
  uint8_t _pin;                                                     // Attribute for pin number of the sensor
  uint16_t _avarage_quantity = 250;                                 // Ammount of read itterations to calculate avverage value
  uint32_t _summary;                                                // Attribute for calulating avverage value
  float _value;                                                     // Last calculated sensor value (raw)
  float _mamps;                                                     // Last calculated value from the sensor in milliamperes
  float _lumens;                                                    // Las calculated value from the sensor in lumens
public:
  LightSensor(uint8_t pin) : _pin(pin){pinMode(_pin, INPUT);}       // Creates object and configures sensors pin to input.
  float get_raw_value();                                            // Method returns raw value from light sensor (0 - 1023)
  float get_lumen_value();                                          // Method returns calculated value from light sensor in lumens
  void set_avverage(uint16_t quantity);                             // Method to set up itteration quantity for average value calculation
};


void LightSensor::set_avverage(uint16_t quantity){
  _avarage_quantity = quantity;
  #ifdef DEBUG
        Serial.print("New itaration number is ");
        Serila.println(_avarage_quantity);
    #endif
}

float LightSensor::get_raw_value(){
  for (uint16_t _i = 0;_i<_avarage_quantity;_i++){
    _summary += analogRead(_pin);
  }
  _value = _summary/_avarage_quantity;
  _summary=0;
  return _value;
}


float LightSensor::get_lumen_value(){
  _value = get_raw_value();
  _mamps = _value * 5 / 1024 * 100;
  _lumens = _mamps * 2;
  return _lumens;

}