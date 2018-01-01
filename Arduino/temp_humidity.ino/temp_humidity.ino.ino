#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>

#include "config.h"
#include "domoticzWifiClient.h"
#include "espWifiServer.h"
#include "ds18b20Sensor.h"
#include "dhtSensor.h"

// for stack analytics
extern "C" {
#include <cont.h>
    extern cont_t g_cont;
}

const char *ssid = "EspSensor";

DomoticzWifiClient wifiClient;
Config* config = new Config();
Sensor* sensor[4];
EspWifiServer* wifiServer = new EspWifiServer(config);

void setup()
{
  Serial.begin(115200);
  delay(10);
  // Wait for serial to initialize.
  while(!Serial) { }
  Serial.println("ESP8266 Initliazing..");

  if (config->readJson()) {      
      wifiClient.setWifiConfig(&config->wifiConfig);
      
      for (uint8_t i = 0; i < config->getNoOfSensors(); i++)
      {
          auto cfg = config->sensorConfig[i];
          if (cfg.type == DHT11) {
              sensor[i] = new DhtSensor(&wifiClient, cfg.pin, static_cast<int>(DHT11), cfg.domoticz_idx, cfg.domoticz_setpoint_idx);
              Serial.println("DHT11 sensor added!");
          }
          else if (cfg.type == DHT22) {
              sensor[i] = new DhtSensor(&wifiClient, cfg.pin, static_cast<int>(DHT22), cfg.domoticz_idx, cfg.domoticz_setpoint_idx);
              Serial.println("DHT22 sensor added!");
          }
          else if (cfg.type == DS18B20) {
              sensor[i] = new Ds18b20Sensor(&wifiClient, cfg.pin, cfg.domoticz_idx, cfg.domoticz_setpoint_idx);
              Serial.println("DS18B20 sensor added!");
          }
      }
      Serial.println("ESP Configured from json!");
      wifiClient.connect();
      
      Serial.println("wifiServer started");
  }
  else {
      Serial.print("Setting up softAP ... ");
      Serial.println(WiFi.softAP(ssid) ? "OK" : "Failed");
      Serial.print("Server IP address: ");
      Serial.println(WiFi.softAPIP());
      Serial.print("Server MAC address: ");
      Serial.println(WiFi.softAPmacAddress());
  }
  if (!MDNS.begin(config->wifiConfig.dns)) {             // Start the mDNS responder (default esp8266.local )
      Serial.println("Error setting up MDNS responder!");
  }
  Serial.println("mDNS responder started");

  wifiServer->start();  
}

int seconds = 0;
void loop()
{    
    if (seconds >= config->sleepConfig.sleepTime) {
        seconds = 0;
    }
    if (seconds == 0)
    {
        /*Serial.printf("Free heap: %u\n", ESP.getFreeHeap());
        Serial.printf("Free stack: %4d\n", cont_get_free_stack(&g_cont));*/
        // Get readings from all sensors
        for (uint8_t i = 0; i < config->getNoOfSensors(); i++)
        {
            sensor[i]->read();
            sensor[i]->updateData();
            sensor[i]->getSetPointVal();
            /*Serial.printf("Free read: %u\n", ESP.getFreeHeap());
            Serial.printf("Free read: %4d\n", cont_get_free_stack(&g_cont));*/
        }
        /*Serial.printf("Free after: %u\n", ESP.getFreeHeap());
        Serial.printf("Free after: %4d\n", cont_get_free_stack(&g_cont));*/
    }

    wifiServer->handle();
    

    delay(1000); // Wait 1 second
    seconds++;

    //if (config.sleepConfig.deepSleep) {
    //  Serial.println("Deepsleep enabled");
    //  ESP.deepSleep(config.sleepConfig.sleepTime * 1000000);
    //}
    //else {
    //  Serial.println("Deepsleep not enabled");  
    //  delay(config.sleepConfig.sleepTime * 1000); // Wait configured time 
    //}
}
