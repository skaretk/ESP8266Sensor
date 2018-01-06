// Class for read DHT temperature and humidity Sensor
#pragma once
#include <DHT.h>
#include "sensor.h"

class DhtSensor : public Sensor
{
public:    
    DhtSensor(DomoticzWifiClient* wifiClient, int pin, uint8_t type, DomoticzData_t domoticzData) : Sensor(wifiClient, pin, "DHT"), m_type(type), m_domoticz(domoticzData), dht(pin, type) {
        dht.begin();
    };    

    String data() override;
    
private:
    void read() override;
    void updateData() override;    
    void print() override;
    void getSetPointVal() override;

    DomoticzData_t m_domoticz{};

    uint8_t m_type;
    DHT dht;
    float m_temp {};
    float m_humidity {};
    float m_hic {}; // heat index
};
