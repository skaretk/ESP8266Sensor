#include "dhtSensor.h"

void DhtSensor::read()
{
    // Reading temperature or humidity takes about 250 milliseconds!
    // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
    float humidity = dht.readHumidity();
    // Read temperature as Celsius (the default)
    float temp = dht.readTemperature();

    // Check if any reads failed and exit early (to try again).
    if (isnan(humidity) || isnan(temp)) {
        Serial.println("Failed to read from DHT sensor!");
        return;
    }

    m_humidity = humidity;
    m_temp = temp;

    Serial.print("Humidity: ");
    Serial.print(m_humidity);
    Serial.print(" %\t");
    Serial.print("Temperature: ");
    Serial.print(m_temp);
    Serial.print(" *C ");
    Serial.println("");
}

void DhtSensor::updateData()
{
    m_wifiClient->updateData(m_temp, m_humidity, m_domoticz.m_idx);
}

String DhtSensor::data()
{
    String setPoint;
    if (!m_domoticz.setpointIdx)
        setPoint = "";
    else
        setPoint = String("\nSetPoint: " + String(m_domoticz.setpointVal));

    return String("Temperature: " + String(m_temp) + "\nHumidity: " + String(m_humidity) + setPoint);
}

void DhtSensor::print()
{
    Serial.println(data());
}

void DhtSensor::getSetPointVal()
{
    if (m_domoticz.setpointIdx) {
        const char* data = m_wifiClient->getJsonResultData(m_domoticz.setpointIdx);
        m_domoticz.setpointVal = atof(data);
    }
}