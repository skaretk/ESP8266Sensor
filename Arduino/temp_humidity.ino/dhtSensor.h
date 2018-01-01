// Class for read DHT temperature and humidity Sensor
#pragma once
#include <DHT.h>
#include "sensor.h"

class DhtSensor : public Sensor
{
public:    
    DhtSensor(DomoticzWifiClient* wifiClient, int pin, uint8_t type, int idx, int setPoint = 0) : Sensor(wifiClient, pin, "DHT", idx, setPoint), m_type(type), dht(pin, type) {
        dht.begin();
    };    

    String data() override;
    
private:
    void read() override;
    void updateData() override;    
    void print() override;
    void getSetPointVal() override;

    uint8_t m_type;
    DHT dht;
    float m_temp {};
    float m_humidity {};
    float m_hic {}; // heat index
};
