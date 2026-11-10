#include <stdio.h>

enum class Season {
    Summer,
    Winter
};

enum class TimeOfDay {
    Day,
    Night
};

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

class Time {
    int hours; //через get_time///////////////////////////////////////////////////
    int minutes;
public:
    bool is_day() const;
    void get_time() {}
};

bool Time::is_day() const {
    int current_minutes = hours * 60 + minutes;
    if (current_minutes >= 480 && current_minutes < 1140) {
        return true;
    }
    return false;
}

class ClimateScheduel {
private:
    Season current_season;
    TimeOfDay current_time;

    ClimateSet summer_day;
    ClimateSet summer_night;
    ClimateSet winter_day;
    ClimateSet winter_night;

public:

    ClimateScheduel();
    void set_season(Season season);
    void update_time(const Time& time);
    const ClimateSet& get_set() const;

};

ClimateScheduel::ClimateScheduel() {
    current_season = Season::Summer;
    current_time = TimeOfDay::Day;

    summer_day = {28, 20, 70, 40, 70, 40, 3000, 500};
    summer_night = {23, 15, 80, 40, 70, 40, 0, 0};

    winter_day = {25, 18, 70, 40, 70, 40, 3000, 500};
    winter_night = {20, 12, 80, 40, 70, 40, 0, 0};
}

void ClimateScheduel::set_season(Season season) {
    current_season = season;
}

void ClimateScheduel::update_time(const Time& time) {
    if (time.is_day()) {
        current_time = TimeOfDay::Day;
    }
    else {
        current_time = TimeOfDay::Night;
    }
}

const ClimateSet& ClimateScheduel::get_set() const {
    if (current_season == Season::Summer) {
        if (current_time == TimeOfDay::Day) { return summer_day; }
        else { return summer_night; }
    }
    else {
        if (current_time == TimeOfDay::Day) { return winter_day; }
        else { return winter_night; }
    }
}


//FanScheduel

//////////////////////////////sensores//

class Thermometer {
public:
    int temperature;  // Celcius
public:
    Thermometer() { temperature = 0; }
public:
    void get_temperature() { temperature = 30; }  // TODO: arduino
};

class AirHumidity {
public:
    int air_humidity;
public:
    AirHumidity() { air_humidity = 0; }
public:
    void get_air_humidity() { air_humidity = 30; }  // TODO: arduino
};

class SoilHumidity {
public:
    int soil_humidity;
public:
    SoilHumidity() { soil_humidity = 0; }
public:
    void get_soil_humidity() { soil_humidity = 30; }  // TODO: arduino
};

class Light {
public:
    int light;  // Candel
public:
    Light() { light = 0; }
public:
    void get_light() { light = 1000; }  // TODO: arduino
};

/////////////////////////actuators//

class Heater {
private:
    int pin;
public:
    bool on_heater;
    Heater(int pin) : pin(pin) { on_heater = false; }
    void power();
};

class Pump {
private:
    int pin;
public:
    bool on_pump;
    Pump(int pin) : pin(pin) { on_pump = false; }  //todo arduino
    void power(); //todo
};

class Fan {
private:
    int pin;
public:
    bool on_fan;
    Fan(int pin) : pin(pin) { on_fan = false; }
    void power();
};

class Lamp {
private:
    int pin;
public:
    bool on_light;
    Lamp(int pin) : pin(pin) { on_light = false; };  // TODO: arduino
    void power();  // TODO: arduino
};

/////////////////////////power functions

void Heater::power() {
    if (on_heater) {}
    else {}
}

void Pump::power() {
    if (on_pump) {}
    else {}
}

void Fan::power() {
    if (on_fan) {}
    else {}
}

void Lamp::power()
{
    if (on_light) { /* arduino code */ }
    else { /* arduino code */ }
}

/////////////////////////////////control functions

void control_temperature(const ClimateSet& climate, const Thermometer& temperature, Heater& heater, Fan& fan) { 
    if (temperature.temperature > climate.max_temperature) {
        fan.on_fan = true;
        return;
    }

    if (temperature.temperature < climate.min_temperature) {
        fan.on_fan = true;
        heater.on_heater = true; 
        return;
    }

    fan.on_fan = false;
    heater.on_heater = false;
}

void control_air_humidity(const ClimateSet& climate, const AirHumidity& air_humidity, Pump& pump, Fan& fan, Heater& heater) { 
    if (air_humidity.air_humidity <  climate.min_air_humidity) {
        pump.on_pump = true;
        return;
    }
    if (air_humidity.air_humidity > climate.max_air_humidity) {
        fan.on_fan = true;
        heater.on_heater = true; 
        return;
    }

    fan.on_fan = false;
    heater.on_heater = false;
    pump.on_pump = false;
}

void control_soil_humidity(const ClimateSet& climate, const SoilHumidity& soil_humidity, const Thermometer& temperature, Pump& pump) { 
    if (soil_humidity.soil_humidity < climate.min_soil_humidity) {
        pump.on_pump = true;
        return;
    }
    pump.on_pump = false;
}

void control_light(const ClimateSet& climate, const Light& light, Lamp& lamp, const Time& time)
{
    if ( light.light > climate.min_illuminance && time.is_day()) {
        lamp.on_light = false;
        return;
    }
    
    lamp.on_light = true;
}

//////////////////////////////////////////////////////////////////////////////

int main()
{
    Thermometer thermometer;
    AirHumidity air_humidity;
    SoilHumidity soil_humidity;
    Light light;
    

    ClimateScheduel climate;
    Time time;
    climate.set_season(Season::Summer);
    time.get_time();
    climate.update_time(time);
    const ClimateSet& set = climate.get_set();
    
    Lamp lamp(10);
    Pump pump(11);
    Fan fan(12);
    Heater heater(13);

    for (int count = 10; count != 0; --count) {

        thermometer.get_temperature();
        air_humidity.get_air_humidity();
        soil_humidity.get_soil_humidity();
        light.get_light();

        control_temperature(set, thermometer, heater, fan);
        control_air_humidity(set, air_humidity, pump, fan, heater);
        control_soil_humidity(set, soil_humidity, thermometer, pump);
        control_light(set, light, lamp, time);

        // heater.power();
        // fan.power();
        lamp.power();
        // pump.power();        
    }
}
