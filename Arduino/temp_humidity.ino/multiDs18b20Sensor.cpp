#include "multiDs18b20Sensor.h"

void MultiDs18b20Sensor::setDeviceAddress(DeviceAddress addr, int no)
{
    if (no == 0) {
        memcpy(m_deviceAddress0, addr, sizeof(addr));
    }
    else if (no == 1) {
        memcpy(m_deviceAddress1, addr, sizeof(addr));
    }
    else
        Serial.println("multiDs18b20Sensor: Invalid number");  
}

bool MultiDs18b20Sensor::resolve()
{
    oneWire.reset_search();
    if (!dallasTemp.getAddress(m_deviceAddress0, 0)) Serial.println("Unable to find address for Device 0");
    if (!dallasTemp.getAddress(m_deviceAddress1, 1)) Serial.println("Unable to find address for Device 1");
}


void MultiDs18b20Sensor::read()
{
    Serial.print("Requesting multiple dallas temp...");
    m_temp[0] = dallasTemp.getTempC(m_deviceAddress0);
    m_temp[1] = dallasTemp.getTempC(m_deviceAddress1);
    Serial.println("DONE");
    print();
}

void MultiDs18b20Sensor::updateData()
{
    m_wifiClient->updateData(m_temp[0], m_idx);
    m_wifiClient->updateData(m_temp[1], m_idx2);
}

String MultiDs18b20Sensor::data()
{
    return String("Temperatures: Device 0: " + String(m_temp[0]) + " Device 1: " + String(m_temp[1]));
}

void MultiDs18b20Sensor::print()
{
    Serial.println(data());
}

void MultiDs18b20Sensor::getSetPointVal()
{
    const char* data = m_wifiClient->getJsonResultData(m_setpoint_idx);
    Serial.println(data);
}
