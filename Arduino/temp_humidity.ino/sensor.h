// Base Class for Sensor
#pragma once
#include "domoticzWifiClient.h"

class Sensor
{
public:
    const char* sensorType;

    virtual void read() = 0;
    virtual void updateData() = 0;
    virtual String data() = 0;
    virtual void print() = 0;
    virtual void getSetPointVal() = 0;

protected:
    Sensor(DomoticzWifiClient* wifiClient, int pin, const char* type, int idx, int setPoint) :m_wifiClient(wifiClient), m_pin(pin), sensorType(type), m_idx(idx), m_setpoint_idx(setPoint) {}
    DomoticzWifiClient* m_wifiClient;
    int m_pin;
    int m_idx;
    int m_setpoint_idx;
};
