// Class for read DS18B20 temperature Sensor
#pragma once
#include <DallasTemperature.h>
#include <OneWire.h>
#include "sensor.h"

class Ds18b20Sensor : public Sensor
{
public:
    Ds18b20Sensor(int pin, int idx, WifiClient* wifiClient) : Sensor(pin, idx, "DS18", wifiClient), oneWire(pin), dallasTemp(&oneWire) {
        dallasTemp.begin();
    };
    void read() override;
    void updateData() override;
private:
    OneWire oneWire;
    DallasTemperature dallasTemp;
    float m_temp {};
};
