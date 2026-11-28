#include <Arduino.h>
#include <DHT.h>

#define HEATER_PIN 3
#define PUMP_PIN 5
#define LAMP_PIN 6
#define FAN_PIN 7

#define DHT_PIN 8
#define SOIL_PIN A0
#define LIGHT_PIN A1

#define DHT_TYPE DHT11

///////////////////предварительно
class Time;
struct ClimateSet;
class ClimateSchedule;
struct FanRequest;
class FanSchedule;
struct HeatRequest;
class Thermometer;
class AirHumidity;
class SoilHumidity;
class Light;
class Heater;
class Pump;
class Fan;
class Lamp;
///////////////////

DHT dht(DHT_PIN, DHT_TYPE);

enum class Season {
    Summer,
    Winter
};
enum class TimeOfDay {
    Day,
    Night
};

//////////////////

class Time {
    unsigned long start_millis;

public:
    void begin() { start_millis = millis(); }
    bool is_day() const;
};

bool Time::is_day() const { // UL - приведение типов в unsigned long
    unsigned long elapsed = (millis() - start_millis) % 86400000UL;
    return elapsed < 11UL * 60 * 60 * 1000;
}

//////////////////

struct ClimateSet {
    int max_temperature;
    int min_temperature;
    int max_air_humidity;
    int min_air_humidity;
    int max_soil_humidity;
    int min_soil_humidity;
    int max_illuminance;
    int min_illuminance;
};

class ClimateSchedule {
private:
    Season current_season;
    TimeOfDay current_time;

    ClimateSet summer_day;
    ClimateSet summer_night;
    ClimateSet winter_day;
    ClimateSet winter_night;

public:

    ClimateSchedule();
    void set_season(Season season);
    void update_time(const Time& time);
    const ClimateSet& get_set() const;

};

ClimateSchedule::ClimateSchedule() {
    current_season = Season::Summer;
    current_time = TimeOfDay::Day;

    summer_day = {28, 20, 70, 40, 70, 40, 3000, 500};
    summer_night = {23, 15, 80, 40, 70, 40, 0, 0};

    winter_day = {25, 18, 70, 40, 70, 40, 3000, 500};
    winter_night = {20, 12, 80, 40, 70, 40, 0, 0};
}

void ClimateSchedule::set_season(Season season) {
    current_season = season;
}

void ClimateSchedule::update_time(const Time& time) {
    if (time.is_day()) {
        current_time = TimeOfDay::Day;
    }
    else {
        current_time = TimeOfDay::Night;
    }
}

const ClimateSet& ClimateSchedule::get_set() const {
    if (current_season == Season::Summer) {
        if (current_time == TimeOfDay::Day) { return summer_day; }
        else { return summer_night; }
    }
    else {
        if (current_time == TimeOfDay::Day) { return winter_day; }
        else { return winter_night; }
    }
}

//////////////////

struct FanRequest {
    bool temperature = false;
    bool air_humidity = false;
    bool schedule = false;
};

class FanSchedule {
private:
    unsigned long interval;
    unsigned long duration;
    unsigned long last_start;
    bool ventilation_active;
    bool initialized;

public:
    FanSchedule() : interval(2UL * 60 * 60 * 1000), duration(10UL * 60 * 1000), last_start(0), ventilation_active(false), initialized(false) {}
      bool update(const Time& time, unsigned long current_millis);
};

bool FanSchedule::update(const Time& time, unsigned long current_millis) {
    if (!time.is_day()) {
        ventilation_active = false;
        last_start = current_millis;
        initialized = true;
        return false;
    }
    if (!initialized) {
        last_start = current_millis;
        initialized = true;
        return false;
    }
    if (ventilation_active) {
        if (current_millis - last_start >= duration) { ventilation_active = false; }
        return ventilation_active;
    }
    if (current_millis - last_start >= interval) {
        ventilation_active = true;
        last_start = current_millis;
    }

    return ventilation_active;
}

/////////////////sensors

struct HeatRequest {
    bool temperature = false;
    bool humidity = false;
};

///////////////map-функции (по-хорошему, калибровать)

int map_soil_humidity(int raw_value) {
  int humidity = map(raw_value, 0, 1023, 0, 100);
  return constrain(humidity, 0, 100);
}

int map_light(int raw_value) {
    int light_level = map(raw_value, 0, 1023, 0, 3000);
    return light_level;
//    return constrain(light_level, 0, 3000);
}

////////////////

class Thermometer {
public:
    float temperature;  // Celcius
public:
    Thermometer() { temperature = 0; }
public:
    void get_temperature() { temperature = dht.readTemperature(); }  // TODO: arduino
};

class AirHumidity {
public:
    float air_humidity;
public:
    AirHumidity() { air_humidity = 0; }
public:
    void get_air_humidity() { air_humidity = dht.readHumidity(); }  // TODO: arduino
};

class SoilHumidity {
public:
    int soil_humidity;
public:
    SoilHumidity() { soil_humidity = 0; }
public:
    void get_soil_humidity() { int raw_value = analogRead(A0); soil_humidity = map_soil_humidity(raw_value); }
};

class Light {
public:
    int light;  // Candel
public:
    Light() { light = 0; }
public:
    void get_light() { int raw_value = analogRead(A1); light = map_light(raw_value); }
};

////////////////actuators

class Heater {
private:
    int pin;
public:
    bool on_heater;
    Heater(int pin) : pin(pin), on_heater(false) {}
    void power();
};

class Pump {
private:
    int pin;
public:
    bool on_pump;
    Pump(int pin) : pin(pin), on_pump(false) {} 
    void power(); 
};

class Fan {
private:
    int pin;
public:
    bool on_fan;
    Fan(int pin) : pin(pin), on_fan(false) {}
    void power();
};

class Lamp {
private:
    int pin;
public:
    bool on_light;
    Lamp(int pin) : pin(pin), on_light(false) {}; 
    void power();  
};

///////////////power

void Heater::power() {
    if (on_heater) {digitalWrite(pin, HIGH);}
    else {digitalWrite(pin, LOW);}
}

void Pump::power() {
    if (on_pump) {digitalWrite(pin, HIGH);}
    else {digitalWrite(pin, LOW);}
}

void Fan::power() {
    if (on_fan) {digitalWrite(pin, HIGH);}
    else {digitalWrite(pin, LOW);}
}

void Lamp::power(){
    if (on_light) {digitalWrite(pin, HIGH);}
    else {digitalWrite(pin, LOW);}
}

///////////////control

void control_temperature(const ClimateSet& climate, const Thermometer& temperature, HeatRequest& H_request, FanRequest& requests) { 
    if (temperature.temperature > climate.max_temperature) {
        requests.temperature = true;
        return;
    }

    if (temperature.temperature < climate.min_temperature) {
        requests.temperature = true;
        H_request.temperature = true; 
        return;
    }

    requests.temperature = false;
    H_request.temperature = false;
}

void control_air_humidity(const ClimateSet& climate, const AirHumidity& air_humidity, FanRequest& requests, HeatRequest& H_request) { 
        if (air_humidity.air_humidity > climate.max_air_humidity) {
        requests.air_humidity = true;
        H_request.humidity = true; 
        return;
    }

    requests.air_humidity = false;
    H_request.humidity = false;
}

void control_soil_humidity(const ClimateSet& climate, const SoilHumidity& soil_humidity, Pump& pump) { 
    if (soil_humidity.soil_humidity < climate.min_soil_humidity) {
        pump.on_pump = true;
        return;
    }
    pump.on_pump = false;
}

void control_light(const ClimateSet& climate, const Light& light, Lamp& lamp, const Time& time) {
    lamp.on_light = time.is_day() && light.light < climate.min_illuminance;
}

///////////////update

void update_fan(const FanRequest& requests, Fan& fan) {
    fan.on_fan = requests.temperature || requests.air_humidity || requests.schedule;
}

void update_heater(const HeatRequest& requests, Heater& heater) {
    heater.on_heater = requests.temperature || requests.humidity;
}

///////////////

Thermometer thermometer;
AirHumidity air_humidity;
SoilHumidity soil_humidity;
Light light;

FanRequest requests;
HeatRequest H_requests;

FanSchedule fan_schedule;
ClimateSchedule climate;
Time time;

Lamp lamp(LAMP_PIN);
Pump pump(PUMP_PIN);
Fan fan(FAN_PIN);
Heater heater(HEATER_PIN);

void setup() {
    time.begin();
    dht.begin();

    pinMode(HEATER_PIN, OUTPUT);
    pinMode(PUMP_PIN, OUTPUT);
    pinMode(LAMP_PIN, OUTPUT);
    pinMode(FAN_PIN, OUTPUT);

    digitalWrite(HEATER_PIN, LOW); // проверить не инвертировано ли реле
    digitalWrite(PUMP_PIN, LOW);
    digitalWrite(LAMP_PIN, LOW);
    digitalWrite(FAN_PIN, LOW);    
    
    climate.set_season(Season::Summer);  
}

////////////////


void loop() {
    climate.update_time(time);

    requests = FanRequest{};
    H_requests = HeatRequest{};

    const ClimateSet& set = climate.get_set();
    requests.schedule = fan_schedule.update(time, millis());

    thermometer.get_temperature();
    air_humidity.get_air_humidity();
    soil_humidity.get_soil_humidity();
    light.get_light();

    control_temperature(set, thermometer, H_requests, requests);
    control_air_humidity(set, air_humidity, requests, H_requests);
    control_soil_humidity(set, soil_humidity, pump);
    control_light(set, light, lamp, time);

    update_fan(requests, fan);
    update_heater(H_requests, heater);

    heater.power();
    fan.power();
    lamp.power();
    pump.power();

    delay(1000);
}