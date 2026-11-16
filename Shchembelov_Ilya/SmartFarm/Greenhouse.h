class Greenhouse {                                                      // Creating class for greenhouse boundary conditions
private:
  float _temp_high = 28.0;                                              // Air temperature above this value is considered high
  float _temp_low = 18.0;                                               // Air temperature below this value is considered low
  float _humid_high = 80.0;                                             // Air humidity above this value is considered high
  float _humid_low = 40.0;                                              // Air humidity below this value is considered low
  float _light_on_level = 100.0;                                        // Light level below this value turns the grow light on
  float _light_off_level = 200.0;                                       // Light level above this value turns the grow light off
  uint32_t _watering_time = 5000;                                       // Pump run time for one watering in milliseconds
  uint32_t _soaking_time = 60000;                                       // Pause after watering before the next soil check in milliseconds
  uint8_t _soil_dry_level = 0;                                          // Soil moisture level that requires watering
public:
  Greenhouse() {}                                                       // Creates object with default boundary values
  void set_temp_high(float value);                                      // Method to set up high temperature border
  void set_temp_low(float value);                                       // Method to set up low temperature border
  void set_humid_high(float value);                                     // Method to set up high humidity border
  void set_humid_low(float value);                                      // Method to set up low humidity border
  void set_light_on_level(float value);                                 // Method to set up light level for turning the grow light on
  void set_light_off_level(float value);                                // Method to set up light level for turning the grow light off
  void set_watering_time(uint32_t value);                               // Method to set up watering time in milliseconds
  void set_soaking_time(uint32_t value);                                // Method to set up pause after watering in milliseconds
  void set_soil_dry_level(uint8_t value);                               // Method to set up soil moisture level that requires watering
  float get_temp_high();                                                // Method returns high temperature border
  float get_temp_low();                                                 // Method returns low temperature border
  float get_humid_high();                                               // Method returns high humidity border
  float get_humid_low();                                                // Method returns low humidity border
  float get_light_on_level();                                           // Method returns light level for turning the grow light on
  float get_light_off_level();                                          // Method returns light level for turning the grow light off
  uint32_t get_watering_time();                                         // Method returns watering time in milliseconds
  uint32_t get_soaking_time();                                          // Method returns pause after watering in milliseconds
  uint8_t get_soil_dry_level();                                         // Method returns soil moisture level that requires watering
};


void Greenhouse::set_temp_high(float value){
  _temp_high = value;
  #ifdef DEBUG
    Serial.print("New high temperature border is set to ");
    Serial.println(_temp_high);
  #endif
}

void Greenhouse::set_temp_low(float value){
  _temp_low = value;
  #ifdef DEBUG
    Serial.print("New low temperature border is set to ");
    Serial.println(_temp_low);
  #endif
}

void Greenhouse::set_humid_high(float value){
  _humid_high = value;
  #ifdef DEBUG
    Serial.print("New high humidity border is set to ");
    Serial.println(_humid_high);
  #endif
}

void Greenhouse::set_humid_low(float value){
  _humid_low = value;
  #ifdef DEBUG
    Serial.print("New low humidity border is set to ");
    Serial.println(_humid_low);
  #endif
}

void Greenhouse::set_light_on_level(float value){
  _light_on_level = value;
  #ifdef DEBUG
    Serial.print("New light level for turning the grow light on is set to ");
    Serial.println(_light_on_level);
  #endif
}

void Greenhouse::set_light_off_level(float value){
  _light_off_level = value;
  #ifdef DEBUG
    Serial.print("New light level for turning the grow light off is set to ");
    Serial.println(_light_off_level);
  #endif
}

void Greenhouse::set_watering_time(uint32_t value){
  _watering_time = value;
  #ifdef DEBUG
    Serial.print("New watering time is set to ");
    Serial.println(_watering_time);
  #endif
}

void Greenhouse::set_soaking_time(uint32_t value){
  _soaking_time = value;
  #ifdef DEBUG
    Serial.print("New pause after watering is set to ");
    Serial.println(_soaking_time);
  #endif
}

void Greenhouse::set_soil_dry_level(uint8_t value){
  _soil_dry_level = value;
  #ifdef DEBUG
    Serial.print("New soil moisture level that requires watering is set to ");
    Serial.println(_soil_dry_level);
  #endif
}

float Greenhouse::get_temp_high(){
  return _temp_high;
}

float Greenhouse::get_temp_low(){
  return _temp_low;
}

float Greenhouse::get_humid_high(){
  return _humid_high;
}

float Greenhouse::get_humid_low(){
  return _humid_low;
}

float Greenhouse::get_light_on_level(){
  return _light_on_level;
}

float Greenhouse::get_light_off_level(){
  return _light_off_level;
}

uint32_t Greenhouse::get_watering_time(){
  return _watering_time;
}

uint32_t Greenhouse::get_soaking_time(){
  return _soaking_time;
}

uint8_t Greenhouse::get_soil_dry_level(){
  return _soil_dry_level;
}
