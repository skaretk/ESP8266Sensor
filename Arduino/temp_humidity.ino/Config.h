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

struct WifiCfg {
    // Wifi
    String ssid = "KogC";
    String password = "Vesla2011"; 
    // Domoticz
    String domoticz_ip = "192.168.1.35";
    int domoticz_port = 8080;
    int domoticz_idx = 125;
};

struct SleepCfg {
    bool deepSleep = false;
    int sleepTime = 60; // seconds
};

struct DhtCfg {
    uint8_t pin = 4; 		// D2
    uint8_t type = DHT11;
};

struct LedCfg {
  bool enabled = false;
};

class Config
{
public:
    Config() {};
    bool readJson();
    void writeJson();

    bool deserialize(char* json);
    //void serialize(const WifiCfg& data, char* json, size_t maxSize);
    
    WifiCfg getWifi() { return wifiConfig; }
    SleepCfg getSleep() {return sleepConfig; }
    DhtCfg getDht() { return dhtConfig; }
    LedCfg getLed() { return ledConfig; }
    
private:
    WifiCfg wifiConfig;
    SleepCfg sleepConfig;
    DhtCfg dhtConfig;
    LedCfg ledConfig;	
    bool configured{ false };	
};

#endif
