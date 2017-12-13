/* Config.h
 * Basig configuration for the ESP
*/
#ifndef CONFIG_H
#define CONFIG_H
#include <DHT.h>
#include <ArduinoJson.h>

// #define DHTTYPE DHT11   // DHT 11
// #define DHTTYPE DHT22   // DHT 22 (AM2302), AM2321
// #define DHTTYPE DHT21   // DHT 21 (AM2301)
#define DS18B20 99

struct WifiCfg {
    // Wifi
    char ssid[30] = "";
    char password[20] = ""; 
    // Domoticz
    char domoticz_ip[20] = "";
    uint16_t domoticz_port = 0;
};

struct SleepCfg {
    bool deepSleep;
    int sleepTime;  // seconds
};

struct SensorCfg {
    uint8_t pin;
    uint8_t type;
    int domoticz_idx;
};

struct LedCfg {
  bool enabled;
};

class Config
{
public:
    Config() {};
    bool readJson();
    //void writeJson();

    uint8_t getNoOfSensors() const { return m_noOfSensors; }

    bool deserialize(char* json);
    //void serialize(const WifiCfg& data, char* json, size_t maxSize);
    
    WifiCfg wifiConfig;
    SleepCfg sleepConfig;
    SensorCfg sensorConfig[4]; // max 4 sensors
    LedCfg ledConfig;
    
private:    
    uint8_t m_noOfSensors{};
    bool configured{ false };	
};

#endif
