// Class for read multiple DS18B20 temperature Sensor on the same oneWire bus
#pragma once
#include <DallasTemperature.h>
#include <OneWire.h>
#include "sensor.h"

class MultiDs18b20Sensor : public Sensor
{
public:
    MultiDs18b20Sensor(DomoticzWifiClient* wifiClient, int pin, int idx, int idx2, int setPoint = 0) : Sensor(wifiClient, pin, "MULTIDS18", idx, setPoint), oneWire(pin), dallasTemp(&oneWire),
        m_idx2(idx2) {
        dallasTemp.begin();
    };
    void setDeviceAddress(DeviceAddress addr, int no);
    bool resolve();
    String data() override;

private:
    void read() override;
    void updateData() override;
    void print() override;
    void getSetPointVal() override;

    DeviceAddress m_deviceAddress0, m_deviceAddress1; // max two sensors on the same bus
    int m_idx2;

    OneWire oneWire;
    DallasTemperature dallasTemp;
    float m_temp[2];
};
