/// Config.h
/// Basig configuration for the ESP
///

#include "config.h"

bool Config::readJson()
{
    char jsonConfig[] = R"({
        "wifi": {
            "ssid": "",
            "password": "",
            "ip": "",
            "port": ""
        },			
        "sensors": [ {
                "pin": "4",
                "type": "DHT11",
                "idx": "1"
            },
            {
                "pin": "12",
                "type": "DS18B20",
                "idx": "2"
            } ],
        "sleep": {
            "deepSleep": "false",
            "sleepTime": "60"
        },
        "led": {
            "enabled": "false"
        }
    })";
    return deserialize(jsonConfig);
}

bool Config::deserialize(char* json)
{
    Serial.println("deserialize()");
    StaticJsonBuffer<600> jsonBuffer;
    JsonObject& root = jsonBuffer.parseObject(json);
    // Test if parsing succeeds.
    if (!root.success()) {
        Serial.println("parseObject() failed");
        return false;
    }
    root.prettyPrintTo(Serial);

    /// WIFI Config
    JsonObject& wifi = root["wifi"];
    const char* ssid = wifi["ssid"];
    strcpy(wifiConfig.ssid, ssid);    
    const char* pw = wifi["password"];
    strcpy(wifiConfig.password, pw);    
    Serial.println("Wifi CFG OK");

    /// Domoticz Config
    const char* ip = wifi["ip"];
    strcpy(wifiConfig.domoticz_ip, ip);
    wifiConfig.domoticz_port = wifi.get<uint16_t>("port");
    Serial.println("Domoticz CFG OK");

    /// Sensor Config
    JsonArray& sensors = root["sensors"];
    for (auto& sensor : sensors) {
        sensorConfig[m_noOfSensors].pin = sensor["pin"];
        const char* dhtType = sensor["type"];
        Serial.println(dhtType);
        if (strcmp(dhtType, "DHT11") == 0)
            sensorConfig[m_noOfSensors].type = DHT11;
        else if (strcmp(dhtType, "DHT22") == 0)
            sensorConfig[m_noOfSensors].type = DHT22;
        else if (strcmp(dhtType, "DHT21") == 0)
            sensorConfig[m_noOfSensors].type = DHT21;
        else if (strcmp(dhtType, "AM2301") == 0)
            sensorConfig[m_noOfSensors].type = AM2301;
        else if (strcmp(dhtType, "DS18B20") == 0)
            sensorConfig[m_noOfSensors].type = DS18B20;
        else {
            Serial.println("Sensor NOT OK");
            return false;
        }
        sensorConfig[m_noOfSensors].domoticz_idx = sensor["idx"];
        m_noOfSensors++;
        Serial.println("Sensor Added");
    }

    /// Sleep Config
    JsonObject& sleep = root["sleep"];
    sleepConfig.deepSleep = sleep.get<bool>("deepSleep");
    sleepConfig.sleepTime = sleep.get<int>("sleepTime");
    Serial.println("Sleep CFG OK");

    /// LED Config
    JsonObject& led = root["led"];
    ledConfig.enabled = led.get<bool>("enabled");
    Serial.println("LED CFG OK");
    
    return root.success();
}

/*void Config::writeJson()
{
    const size_t bufferSize = 3*JSON_OBJECT_SIZE(2) + JSON_OBJECT_SIZE(3) + JSON_OBJECT_SIZE(4);
    DynamicJsonBuffer jsonBuffer(bufferSize);
    
    JsonObject& root = jsonBuffer.createObject();

    JsonObject& wifi = root.createNestedObject("wifi");
    wifi["ssid"] = "";
    wifi["password"] = "";

    JsonObject& domoticz = root.createNestedObject("domoticz");
    domoticz["ip"] = "";
    domoticz["port"] = "";
    domoticz["idx"] = "";

    JsonObject& esp = root.createNestedObject("esp");
    esp["dhtPin"] = "4";
    esp["dhtType"] = "DHT11";

    JsonObject& sleep = root.createNestedObject("sleep");
    sleep["deepSleep"] = "false";
    sleep["time"] = "60";
}*/

/*void Config::serialize(const WifiCfg& wifi, char* json, size_t maxSize)
{
    StaticJsonBuffer<400> jsonBuffer;
    JsonObject& root = jsonBuffer.createObject();    
    JsonObject& wifi = root.createNestedObject("wifi");
    root["name"] = wifi.ssid;
    root["time"] = wifi.password;
    root.printTo(json, maxSize);
}*/
