/* WifiClient.h
 * Connect and transmit data to Domoticz
*/
#pragma once

#include "config.h"

class WifiClient
{
public:
    WifiClient() {};
    void connect();
    void updateData(float temperature, float humidity, int idx);
    void updateData(float temperature, int idx);
    void setWifiConfig(WifiCfg*);
    
private:
    bool configured = false;
    // Wifi
    char* ssid {};
    char* password {};
    // Domoticz
    char* domoticzIp {};
    uint16_t domoticzPort = 0;
};


