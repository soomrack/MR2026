class Heater {                                                    // Creating class for heater
private:
  uint8_t _pin;                                                   // Attribute for pin number of the heater
  uint8_t _state;                                                 // Current heater state
public:
  Heater(uint8_t pin) : _pin(pin){pinMode(_pin, OUTPUT); off();}  // Creates object and configures heater pin to output
  void on();                                                      // Method to turn the heater on
  void off();                                                     // Method to turn the heater off
  uint8_t get_state();                                            // Method returns current heater state
};


void Heater::on(){
  digitalWrite(_pin, HIGH);
  _state = 1;
  #ifdef DEBUG
    Serial.println("Heater is turned on");
  #endif
}

void Heater::off(){
  digitalWrite(_pin, LOW);
  _state = 0;
  #ifdef DEBUG
    Serial.println("Heater is turned off");
  #endif
}

uint8_t Heater::get_state(){
  #ifdef DEBUG
    Serial.print("Heater state is ");
    Serial.println(_state);
  #endif
  return _state;
}
