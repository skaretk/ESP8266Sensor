#include "dhtSensor.h"

void DhtSensor::read()
{
    // Reading temperature or humidity takes about 250 milliseconds!
    // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
    m_humidity = dht.readHumidity();
    // Read temperature as Celsius (the default)
    m_temp = dht.readTemperature();

    // Check if any reads failed and exit early (to try again).
    if (isnan(m_humidity) || isnan(m_temp)) {
        Serial.println("Failed to read from DHT sensor!");
        return;
    }

    // Compute heat index in Celsius (isFahreheit = false)
    m_hic = dht.computeHeatIndex(m_temp, m_humidity, false);

    Serial.print("Humidity: ");
    Serial.print(m_humidity);
    Serial.print(" %\t");
    Serial.print("Temperature: ");
    Serial.print(m_temp);
    Serial.print(" *C ");
    Serial.print("Heat index: ");
    Serial.print(m_hic);
    Serial.print(" *C ");
    Serial.println("");
}

void DhtSensor::updateData()
{
    m_wifiClient->updateData(m_temp, m_humidity, m_idx);
}