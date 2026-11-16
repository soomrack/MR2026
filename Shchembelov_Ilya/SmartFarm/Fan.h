class Fan {                                                       // Creating class for ventilation fan
private:
  uint8_t _pin;                                                   // Attribute for pin number of the fan
  uint8_t _state;                                                 // Current fan state
public:
  Fan(uint8_t pin) : _pin(pin){pinMode(_pin, OUTPUT); off();}     // Creates object and configures fan pin to output
  void on();                                                      // Method to turn the fan on
  void off();                                                     // Method to turn the fan off
  uint8_t get_state();                                            // Method returns current fan state
};


void Fan::on(){
  digitalWrite(_pin, HIGH);
  _state = 1;
  #ifdef DEBUG
    Serial.println("Fan is turned on");
  #endif
}

void Fan::off(){
  digitalWrite(_pin, LOW);
  _state = 0;
  #ifdef DEBUG
    Serial.println("Fan is turned off");
  #endif
}

uint8_t Fan::get_state(){
  #ifdef DEBUG
    Serial.print("Fan state is ");
    Serial.println(_state);
  #endif
  return _state;
}
