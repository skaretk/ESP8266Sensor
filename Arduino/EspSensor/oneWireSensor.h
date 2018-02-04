// Class for up to for DS18B20 temperature Sensors on a oneWire bus
#pragma once
#include <OneWire.h>
#include <DallasTemperature.h>
#include "sensor.h"

#define MAX_NO_OF_SENSORS 4
#define TEMPERATURE_PRECISION 12 // Temp read takes 750ms

struct Ds18B20Sensor_t {
    DomoticzData_t domoticzData{};
    DeviceAddress devAddr{};
    float m_temp{};
};

class OneWireSensor : public Sensor
{
public:
    OneWireSensor(DomoticzWifiClient* wifiClient, int pin) : Sensor(wifiClient, pin, "MULTIDS18"), oneWire(pin), sensors(&oneWire){
        sensors.begin();
    };
    void addDs18B20Sensor(Ds18B20Sensor_t* sensor, int idx);
    bool resolve();
    String data() override;

private:
    void read() override;
    void updateData() override;
    void print() override;
    void getSetPointVal() override;

    void printAddress(DeviceAddress deviceAddress);
    
    Ds18B20Sensor_t* m_ds18b20Sensor[MAX_NO_OF_SENSORS];
    uint8_t m_noOfSensors{};

    OneWire oneWire;
    DallasTemperature sensors;
};
