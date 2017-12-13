#include <ESP8266WiFi.h>

#include "config.h"
#include "wifiClient.h"
#include "ds18b20Sensor.h"
#include "dhtSensor.h"

WifiClient wifiClient;
Config config;
Sensor* sensor[4];

void setup()
{
  Serial.begin(57600);
  delay(10);
  // Wait for serial to initialize.
  while(!Serial) { }
  if (config.readJson()) {
      Serial.println("ESP Configured from json");
      wifiClient.setWifiConfig(&config.wifiConfig);
      
      for (uint8_t i = 0; i < config.getNoOfSensors(); i++)
      {
          SensorCfg cfg = config.sensorConfig[i];
          if (cfg.type == DHT11) {
              sensor[i] = new DhtSensor(cfg.pin, cfg.domoticz_idx, static_cast<int>(DHT11), &wifiClient);
              Serial.println("DHT11 sensor configured");
          }
          else if (cfg.type == DHT22) {
              sensor[i] = new DhtSensor(cfg.pin, cfg.domoticz_idx, static_cast<int>(DHT22), &wifiClient);
              Serial.println("DHT22 sensor configured");
          }
          else if (cfg.type == DS18B20) {
              sensor[i] = new Ds18b20Sensor(cfg.pin, cfg.domoticz_idx, &wifiClient);
              Serial.println("DS18B20 sensor configured");
          }
      }
  }
  
  Serial.println("ESP8266 Initliazing!");  
  wifiClient.connect();
}

void loop()
{
    // Get readings from all sensors
    for (uint8_t i = 0; i < config.getNoOfSensors(); i++)
    {
        sensor[i]->read();
        sensor[i]->updateData();
    }

    if (config.sleepConfig.deepSleep) {
      Serial.println("Deepsleep enabled");
      ESP.deepSleep(config.sleepConfig.sleepTime * 1000000);
    }
    else {
      Serial.println("Deepsleep not enabled");  
      delay(config.sleepConfig.sleepTime * 1000); // Wait 60 seconds
    }
}
