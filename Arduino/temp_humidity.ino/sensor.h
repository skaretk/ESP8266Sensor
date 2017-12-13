// Base Class for Sensor
#pragma once
#include "wifiClient.h"

class Sensor
{
public:
    const char* sensorType;

    virtual void read() = 0;
    virtual void updateData() = 0;

protected:
    Sensor(int pin, int idx, const char* type, WifiClient* wifiClient) : m_pin(pin), m_idx(idx), sensorType(type), m_wifiClient(wifiClient){}
    WifiClient* m_wifiClient;
    int m_pin;
    int m_idx;
};
