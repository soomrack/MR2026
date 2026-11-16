class Pump {                                                      // Creating class for water pump
private:
  uint8_t _pin;                                                   // Attribute for pin number of the pump
  uint8_t _state;                                                 // Current pump state
public:
  Pump(uint8_t pin) : _pin(pin){pinMode(_pin, OUTPUT); off();}    // Creates object and configures pump pin to output
  void on();                                                      // Method to turn the pump on
  void off();                                                     // Method to turn the pump off
  uint8_t get_state();                                            // Method returns current pump state
};


void Pump::on(){
  digitalWrite(_pin, HIGH);
  _state = 1;
  #ifdef DEBUG
    Serial.println("Pump is turned on");
  #endif
}

void Pump::off(){
  digitalWrite(_pin, LOW);
  _state = 0;
  #ifdef DEBUG
    Serial.println("Pump is turned off");
  #endif
}

uint8_t Pump::get_state(){
  #ifdef DEBUG
    Serial.print("Pump state is ");
    Serial.println(_state);
  #endif
  return _state;
}
