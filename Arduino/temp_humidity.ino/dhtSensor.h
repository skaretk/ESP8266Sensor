// Class for read DHT temperature and humidity Sensor
#pragma once
#include <DHT.h>
#include "sensor.h"

class DhtSensor : public Sensor
{
public:    
    DhtSensor(int pin, int idx, uint8_t type, WifiClient* wifiClient) : Sensor(pin, idx, "DHT", wifiClient), m_type(type), dht(pin, type) {
        dht.begin();
    };

    void read() override;
    void updateData() override;
private:
    uint8_t m_type;
    DHT dht;
    float m_temp {};
    float m_humidity {};
    float m_hic {}; // heat index
};
