/* Config.h
 * Basig configuration for the ESP
*/
#ifndef CONFIG_H
#define CONFIG_H
#include <DHT.h>

// #define DHTTYPE DHT11   // DHT 11
// #define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
// #define DHTTYPE DHT21   // DHT 21 (AM2301)

struct SleepConfig
{
	bool enabled = false;
	int sleepTimeS = 60;
};

struct ESPConfig
{
	SleepConfig sleep;
	uint8_t dhtPin = 4; 		// D2
	uint8_t dhtType = DHT11;
};

#endif
