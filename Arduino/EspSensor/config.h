/* Config.h
 * Basig configuration for the ESP
*/
#ifndef CONFIG_H
#define CONFIG_H
#include <DHT.h>
#include <ArduinoJson.h>
#include <FS.h>   // Include the SPIFFS library

// #define DHTTYPE DHT11   // DHT 11
// #define DHTTYPE DHT22   // DHT 22 (AM2302), AM2321
// #define DHTTYPE DHT21   // DHT 21 (AM2301)
#define DS18B20 99
#define MULTIDS18 100

struct WifiCfg {
    // Wifi
    char ssid[30] = "";
    char password[20] = ""; 
    // Domoticz
    char domoticz_ip[20] = "";
    uint16_t domoticz_port{0};
    // DNS
    char dns[20] = "esp8266";
    bool configured{ false };
};

struct SensorCfg {
    uint8_t pin;
    uint8_t type;    
};

struct DomoticzCfg {
    int idx;
    int setpoint_idx;
};

struct DhtConfig {
    SensorCfg sensor;
    DomoticzCfg domoticzCfg;
};

struct OneWireCfg {
    SensorCfg sensor;
    DomoticzCfg domoticzCfg[4];
};

struct SleepCfg {
    bool deepSleep;
    int sleepTime;  // seconds
};

struct LedCfg {
  bool enabled;
};

class Config
{
public:
    Config() { SPIFFS.begin(); }
    bool readJson();
    //void writeJson();    

    String printConfig();

    uint8_t getNoOfSensors() const { return m_noOfDhtSensors + m_noOfOneWireBus; }
    uint8_t getNoOfDhtSensors() const { return m_noOfDhtSensors; }
    uint8_t getNoOfDallasTemperatureSensors() const { return m_noOfDallasTemperatureSensors; }

    bool deserialize(String& json);
    //void serialize(const WifiCfg& data, char* json, size_t maxSize);
    
    WifiCfg wifiConfig;
    SleepCfg sleepConfig;
    DhtConfig dhtConfig[4]; // max 4 DHT sensors
    OneWireCfg oneWireConfig; // Max 1 onewire bus with up to 4 DS18B20 sensors
    LedCfg ledConfig;

private:    
    uint8_t m_noOfDhtSensors{};
    uint8_t m_noOfOneWireBus{};
    uint8_t m_noOfDallasTemperatureSensors{};
    const String m_cfgFilename{ "/config.json" };
    String m_json{};
};

#endif
