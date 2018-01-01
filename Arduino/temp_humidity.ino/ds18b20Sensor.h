// Class for read DS18B20 temperature Sensor
#pragma once
#include <DallasTemperature.h>
#include <OneWire.h>
#include "sensor.h"

class Ds18b20Sensor : public Sensor
{
public:
    Ds18b20Sensor(DomoticzWifiClient* wifiClient, int pin, int idx, int setPoint = 0) : Sensor(wifiClient, pin, "DS18", idx, setPoint), oneWire(pin), dallasTemp(&oneWire) {
        dallasTemp.begin();
    };

    String data() override;
    
private:
    void read() override;
    void updateData() override;
    void print() override;
    void getSetPointVal() override;

    OneWire oneWire;
    DallasTemperature dallasTemp;
    float m_temp {};
};
