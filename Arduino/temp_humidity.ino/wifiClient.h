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
	void setWifiConfig(const char* ssid, const char* pw);
	void setDomoticzConfig(const char* ip, int port, int idx);
	
	private:
	// Wifi
	char* ssid = "KogC";
	char* password = "Vesla2011"; 
	// Domoticz
	char* domoticzIp = "192.168.1.35";
	int domoticzPort = 8080;
	int domoticzIdx = 125;
};


