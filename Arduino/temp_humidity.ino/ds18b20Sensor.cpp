#include "ds18b20Sensor.h"

void Ds18b20Sensor::read()
{
    Serial.print("Requesting dallas temp...");
    dallasTemp.requestTemperatures(); // Send the command to get temperature readings 
    Serial.println("DONE");
    Serial.print("Temperature is: ");
    m_temp = dallasTemp.getTempCByIndex(0);
    Serial.print(m_temp);
    // Why "byIndex"?  You can have more than one DS18B20 on the same bus. 0 refers to the first IC on the wire 
}

void Ds18b20Sensor::updateData()
{
    m_wifiClient->updateData(m_temp, m_idx);
}

String Ds18b20Sensor::data()
{
    return String("Temperature: " + String(m_temp));
}

void Ds18b20Sensor::print()
{
    Serial.println(data());
}

void Ds18b20Sensor::getSetPointVal()
{
    const char* data = m_wifiClient->getJsonResultData(m_setpoint_idx);
    Serial.println(data);
}
