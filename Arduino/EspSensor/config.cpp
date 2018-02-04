// Config.h
// Basig configuration for the ESP
//

#include "config.h"

bool Config::readJson()
{
    if (SPIFFS.exists(m_cfgFilename)) {                     // Check if config file exists
        Serial.println("config.json found!");
        File file = SPIFFS.open(m_cfgFilename, "r");
        m_json = file.readString();
        file.close();                                       
        return deserialize(m_json);
    }
    else {
        Serial.println("config.json not found");
        return false; // Config file not found
    }
}

String Config::printConfig()
{
    StaticJsonBuffer<600> jsonBuffer;
    JsonObject& root = jsonBuffer.parseObject(m_json);
    // Test if parsing succeeds.
    if (!root.success()) {
        Serial.println("printConfig: parseObject() failed");
        return "";
    }
    String json = "";
    root.prettyPrintTo(json);
    return json;
}

bool Config::deserialize(String& json)
{
    Serial.println("deserialize()");
    StaticJsonBuffer<1000> jsonBuffer;
    JsonObject& root = jsonBuffer.parseObject(json);
    // Test if parsing succeeds.
    if (!root.success()) {
        Serial.println("deserialize - parseObject() failed");
        return false;
    }
    root.prettyPrintTo(Serial);

    // WIFI Config
    JsonObject& wifi = root["wifi"];
    const char* ssid = wifi["ssid"];
    strcpy(wifiConfig.ssid, ssid);    
    const char* pw = wifi["password"];
    strcpy(wifiConfig.password, pw);

    const char* dns = wifi["dns"];
    if (dns) {
        strcpy(wifiConfig.dns, dns);
        Serial.print("\nmDNS: ");
        Serial.println(dns);
    }    
    Serial.println("Wifi CFG OK");

    // Domoticz Config
    const char* ip = wifi["ip"];
    strcpy(wifiConfig.domoticz_ip, ip);
    wifiConfig.domoticz_port = wifi.get<uint16_t>("port");
    Serial.println("Domoticz CFG OK");

    // Sensor Config
    JsonArray& sensors = root["sensors"];
    for (auto& sensor : sensors) {
        const char* sensorType = sensor["type"];
        SensorCfg* sensorCfg;
        DomoticzCfg* domoticzCfg;
        // DHT Sensors
        if (strncmp(sensorType, "DHT", 3) == 0 || strncmp(sensorType, "AM", 2) == 0) {
            sensorCfg = &dhtConfig[m_noOfDhtSensors].sensor;
            domoticzCfg = &dhtConfig[m_noOfDhtSensors].domoticzCfg;
            if (strcmp(sensorType, "DHT11") == 0)
                sensorCfg->type = DHT11;
            else if (strcmp(sensorType, "DHT22") == 0)
                sensorCfg->type = DHT22;
            else if (strcmp(sensorType, "DHT21") == 0)
                sensorCfg->type = DHT21;
            else if (strcmp(sensorType, "AM2301") == 0)
                sensorCfg->type = AM2301;
            else {
                Serial.println("DHT Sensor NOT OK");
                return false;
            }
            domoticzCfg->idx = sensor["idx"];
            const char* setPoint = sensor["setpointIdx"];
            if (setPoint) { // Optional
                domoticzCfg->setpoint_idx = atoi(setPoint);
            }
            m_noOfDhtSensors++;
            Serial.println("DHT sensor added");
        }
        // OneWire / DS18B20
        else if (strcmp(sensorType, "OneWire") == 0) {
            Serial.println("Onewire bus added");
            sensorCfg = &oneWireConfig.sensor;            
            domoticzCfg = &oneWireConfig.domoticzCfg[0];
            JsonArray& ds18b20sensors = sensor["ds18b20sensors"];
            // DS18B20
            if (ds18b20sensors.success()) {
                sensorCfg->type = DS18B20;
                for (auto& ds18b20sensor : ds18b20sensors) {
                    domoticzCfg->idx = ds18b20sensor["idx"];
                    const char* setPoint = ds18b20sensor["setpointIdx"];
                    if (setPoint) { // Optional
                        domoticzCfg->setpoint_idx = atoi(setPoint);
                    }
                    domoticzCfg++;
                    Serial.println("DS18B20 sensor added");
                    m_noOfDallasTemperatureSensors++;
                }
            }
            m_noOfOneWireBus++;            
        }
        else {
            Serial.println("Sensor NOT OK");
            return false;
        }
        sensorCfg->pin = sensor["pin"];    // Common for each sensorCfg    
    }

    // Sleep Config
    JsonObject& sleep = root["sleep"];
    sleepConfig.deepSleep = sleep.get<bool>("deepSleep");
    sleepConfig.sleepTime = sleep.get<int>("sleepTime");
    Serial.println("Sleep CFG OK");

    // LED Config
    JsonObject& led = root["led"];
    ledConfig.enabled = led.get<bool>("enabled");
    Serial.println("LED CFG OK");
    
    return root.success();
}
