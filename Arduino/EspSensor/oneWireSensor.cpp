#include "oneWireSensor.h"

void OneWireSensor::addDs18B20Sensor(Ds18B20Sensor_t* sensor, int idx)
{
    memcpy(&m_ds18b20Sensor[idx], &sensor, sizeof(sensor));
    m_noOfSensors++;
}

bool OneWireSensor::resolve()
{
    if (m_noOfSensors > MAX_NO_OF_SENSORS) {
        Serial.printf("Too many configured devices: %u\nMAX: %u \n", m_noOfSensors, MAX_NO_OF_SENSORS);
        return false;
    }

    if (m_noOfSensors != sensors.getDeviceCount())
        Serial.printf("Configured devices: %u\nFound: %u devices\n",m_noOfSensors,sensors.getDeviceCount());    

    for (int i = 0; i < m_noOfSensors; i++)
    {
        if (!sensors.getAddress(m_ds18b20Sensor[i]->devAddr, i)) 
            Serial.printf("Unable to find address for Device %u\n", i);
        else {
            Serial.printf("DeviceAddress found: ");
            printAddress(m_ds18b20Sensor[i]->devAddr);
            Serial.println();
            // set the resolution to 9 bit per device
            if (!sensors.setResolution(m_ds18b20Sensor[i]->devAddr, TEMPERATURE_PRECISION)) {
                Serial.println("Could not set resolution");
            }
        }
    }
}

void OneWireSensor::read()
{
    Serial.print("Requesting temperatures...");
    sensors.requestTemperatures();
    Serial.println("DONE");

    for (int i = 0; i < m_noOfSensors; i++) {
        float data = sensors.getTempC(m_ds18b20Sensor[i]->devAddr);
        if (data == DEVICE_DISCONNECTED_C) {
            Serial.printf("Device %i disconnected\n", i);
        }            
        else {
            m_ds18b20Sensor[i]->m_temp = data;
        }
    }
    print();
}

void OneWireSensor::updateData()
{
    for (int i = 0; i < m_noOfSensors; i++) {
        m_wifiClient->updateData(m_ds18b20Sensor[i]->m_temp, m_ds18b20Sensor[i]->domoticzData.m_idx);
    }
}

String OneWireSensor::data()
{
    String data;

    for (int i = 0; i < m_noOfSensors; i++) 
    {
        data += String("Temp" + String(i) + ": " + String(m_ds18b20Sensor[i]->m_temp) + "\n");
    }
    return data;
}

void OneWireSensor::print()
{
    Serial.println(data());
}

void OneWireSensor::getSetPointVal()
{
    for (int i = 0; i < m_noOfSensors; i++)
    {
        if (m_ds18b20Sensor[i]->domoticzData.setpointIdx) {
            const char* data = m_wifiClient->getJsonResultData(m_ds18b20Sensor[i]->domoticzData.setpointIdx);
            m_ds18b20Sensor[i]->domoticzData.setpointVal = atof(data);
        }
    }
}

void OneWireSensor::printAddress(DeviceAddress deviceAddress)
{
    for (uint8_t i = 0; i < 8; i++)
    {
        // zero pad the address if necessary
        if (deviceAddress[i] < 16) Serial.print("0");
        Serial.print(deviceAddress[i], HEX);
    }
}
