/* WifiClient.h
 * Connect and transmit data to Domoticz
*/
#ifndef WIFICLIENT_H
#define WIFICLIENT_H

#include "domoticzCfg.h"

struct WifiCfg
{
	char* ssid = "Enter SSID";
	char* password = "Enter Password"; 
};

class WifiClient
{
	public:
	WifiClient() {};
	void connect();
	void updateData(float temperature, float humidity);
	
	private:
	WifiCfg wifiCfg;
	Domoticz domoticz;
};

#endif
