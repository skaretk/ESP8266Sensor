#include "multiDs18b20Sensor.h"

void MultiDs18b20Sensor::addDs18B20Sensor(Ds18B20Sensor_t &sensor, int idx)
{
    m_ds18b20Sensor[idx] = sensor;
    m_noOfSensors++;
}

bool MultiDs18b20Sensor::resolve()
{
    oneWire.reset_search();
    for (int i = 0; i < m_noOfSensors; i++)
    {
        if (!dallasTemp.getAddress(m_ds18b20Sensor[i].devAddr, i)) 
            Serial.printf("Unable to find address for Device %u\n", i);
    }
}


void MultiDs18b20Sensor::read()
{
    Serial.print("Requesting multiple dallas temp...");
    for (int i = 0; i < m_noOfSensors; i++)
    {
        m_temp[i] = dallasTemp.getTempC(m_ds18b20Sensor[i].devAddr);
    }
    Serial.println("DONE");
    print();
}

void MultiDs18b20Sensor::updateData()
{
    for (int i = 0; i < m_noOfSensors; i++)
    {
        m_wifiClient->updateData(m_temp[i], m_ds18b20Sensor[i].domoticzData.m_idx);
    }
}

String MultiDs18b20Sensor::data()
{
    String data;

    for (int i = 0; i < m_noOfSensors; i++)
    {
        data += String("Temperature " + String(i) + ": " + String(m_temp[i]) + "\n");
    }
    return data;
}

void MultiDs18b20Sensor::print()
{
    Serial.println(data());
}

void MultiDs18b20Sensor::getSetPointVal()
{
    for (int i = 0; i < m_noOfSensors; i++)
    {
        if (m_ds18b20Sensor[i].domoticzData.setpointIdx) {
            const char* data = m_wifiClient->getJsonResultData(m_ds18b20Sensor[i].domoticzData.setpointIdx);
            m_ds18b20Sensor[i].domoticzData.setpointVal = atof(data);
        }
    }
}
