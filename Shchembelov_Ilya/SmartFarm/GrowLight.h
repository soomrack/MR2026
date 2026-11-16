class GrowLight {                                                 // Creating class for grow light
private:
  uint8_t _pin;                                                   // Attribute for pin number of the grow light
  uint8_t _state;                                                 // Current grow light state
public:
  GrowLight(uint8_t pin) : _pin(pin){pinMode(_pin, OUTPUT); off();}  // Creates object and configures grow light pin to output
  void on();                                                      // Method to turn the grow light on
  void off();                                                     // Method to turn the grow light off
  uint8_t get_state();                                            // Method returns current grow light state
};


void GrowLight::on(){
  digitalWrite(_pin, HIGH);
  _state = 1;
  #ifdef DEBUG
    Serial.println("Grow light is turned on");
  #endif
}

void GrowLight::off(){
  digitalWrite(_pin, LOW);
  _state = 0;
  #ifdef DEBUG
    Serial.println("Grow light is turned off");
  #endif
}

uint8_t GrowLight::get_state(){
  #ifdef DEBUG
    Serial.print("Grow light state is ");
    Serial.println(_state);
  #endif
  return _state;
}
