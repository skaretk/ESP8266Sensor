// Class for up to for DS18B20 temperature Sensors on a oneWire bus
#pragma once
#include <DallasTemperature.h>
#include <OneWire.h>
#include "sensor.h"

struct Ds18B20Sensor_t {
    DomoticzData_t domoticzData{};
    DeviceAddress devAddr{};
};

class MultiDs18b20Sensor : public Sensor
{
public:
    MultiDs18b20Sensor(DomoticzWifiClient* wifiClient, int pin) : Sensor(wifiClient, pin, "MULTIDS18"), oneWire(pin), dallasTemp(&oneWire){
        dallasTemp.begin();
    };
    void addDs18B20Sensor(Ds18B20Sensor_t &sensor, int idx);
    bool resolve();
    String data() override;

private:
    void read() override;
    void updateData() override;
    void print() override;
    void getSetPointVal() override;
    
    Ds18B20Sensor_t m_ds18b20Sensor[4]; // max 4 sensors

    int m_noOfSensors{};

    OneWire oneWire;
    DallasTemperature dallasTemp;
    float m_temp[4];
};
