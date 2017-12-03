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
    void updateData(float temperature, float humidity);
    void getWifiConfig(WifiCfg);
    void setDomoticzConfig(const char* ip, int port, int idx);
    
private:
    bool configured = false;
    // Wifi
    char* ssid = "";
    char* password = "";
    // Domoticz
    char* domoticzIp = "";
    int domoticzPort = 0;
    int domoticzIdx = 0;
};


