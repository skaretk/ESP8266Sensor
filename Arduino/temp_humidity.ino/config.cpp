/* Config.h
 * Basig configuration for the ESP
*/

#include "config.h"

bool Config::readJson()
{
    char jsonConfig[] = R"({
        "wifi": {
            "ssid": "",
            "password": "",
            "ip": "",
            "port": "",
            "idx": ""
        },			
        "dht": {
            "pin": "4",
            "type": "DHT11"
        },
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
    StaticJsonBuffer<400> jsonBuffer;
    JsonObject& root = jsonBuffer.parseObject(json);
    // Test if parsing succeeds.
    if (!root.success()) {
        Serial.println("parseObject() failed");
        return false;
    }
    /* WIFI Config */
    JsonObject& wifi = root["wifi"];
    strcpy(wifiConfig.ssid, wifi["ssid"]);
    strcpy(wifiConfig.password, wifi["password"]);
    /* Domoticz Config */
    strcpy(wifiConfig.domoticz_ip, wifi["ip"]);
    wifiConfig.domoticz_port = wifi.get<int>("port");
    wifiConfig.domoticz_idx = wifi.get<int>("idx");    
    /* DHT Config */
    JsonObject& dht = root["dht"];
    dhtConfig.pin = dht.get<uint8_t>("pin");
    String dhtType = dht.get<String>("type");
    if (dhtType == "DHT11")
        wifiConfig.domoticz_idx = DHT11;
    else if (dhtType == "DHT22")
        wifiConfig.domoticz_idx = DHT22;
    else if (dhtType == "DHT21")
        wifiConfig.domoticz_idx = DHT21;
    else if (dhtType == "AM2301")
        wifiConfig.domoticz_idx = AM2301;
    else
        return false;
    /* Sleep Config */
    JsonObject& sleep = root["sleep"];
    sleepConfig.deepSleep = sleep.get<bool>("deepSleep");
    sleepConfig.sleepTime = sleep.get<int>("sleepTime");
    /* LED Config */
    JsonObject& led = root["led"];
    ledConfig.enabled = led.get<bool>("enabled");
    root.prettyPrintTo(Serial);
    return root.success();
}

void Config::writeJson()
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
}



/*void Config::serialize(const WifiCfg& wifi, char* json, size_t maxSize)
{
    StaticJsonBuffer<400> jsonBuffer;
    JsonObject& root = jsonBuffer.createObject();    
    JsonObject& wifi = root.createNestedObject("wifi");
    root["name"] = wifi.ssid;
    root["time"] = wifi.password;
    root.printTo(json, maxSize);
}*/
