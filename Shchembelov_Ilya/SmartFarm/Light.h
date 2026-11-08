class LightSensor {
private:
  uint8_t _pin;
  uint16_t _avarage_quantity = 250;
  uint32_t _summary;
  float _value;
  float _mamps;
  float _lumens;
public:
  LightSensor(uint8_t pin) : _pin(pin){pinMode(_pin, INPUT);}
  float get_raw_value();
  float get_lumen_value();
  void set_avverage(uint16_t quantity);
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