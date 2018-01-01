/* WifiClient.h
 * Connect and transmit data to Domoticz
*/
#pragma once

#include "config.h"

class DomoticzWifiClient
{
public:
    DomoticzWifiClient() {};
    void connect();
    void updateData(float temperature, float humidity, int idx);
    void updateData(float temperature, int idx);
    void setWifiConfig(WifiCfg*);
    void getJson(int idx, String& json);
    const char* getJsonResultData(int idx);
    
private:
    // Wifi
    char* ssid {};
    char* password {};
    // Domoticz
    char* domoticzIp {};
    uint16_t domoticzPort = 0;
};


