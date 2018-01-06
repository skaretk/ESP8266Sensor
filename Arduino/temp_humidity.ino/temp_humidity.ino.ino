#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>

// SPI Display SSD1306
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
//----------------------------
#include "config.h"
#include "domoticzWifiClient.h"
#include "espWifiServer.h"
#include "multiDs18b20Sensor.h"
#include "dhtSensor.h"

// for stack analytics
extern "C" {
#include <cont.h>
    extern cont_t g_cont;
}

// SPI Display SSD1306
#define OLED_RESET 0
Adafruit_SSD1306 display(OLED_RESET);

#if (SSD1306_LCDHEIGHT != 64)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif
//----------------------------


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

  // by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3D (for the 128x64)

  display.display();
  delay(500);  
                                              
  Serial.println("ESP8266 Initliazing..");

  if (config->readJson()) {
      wifiClient.setWifiConfig(&config->wifiConfig);
      
      
      for (uint8_t i = 0; i < config->getNoOfSensors(); i++)
      {
          auto cfg = config->sensorConfig[i];
          if (cfg.type == DHT11) {
              DomoticzData_t dht11Domoticz;
              dht11Domoticz.m_idx = cfg.domoticz_idx;
              dht11Domoticz.setpointIdx = cfg.domoticz_setpoint_idx;
              sensor[i] = new DhtSensor(&wifiClient, cfg.pin, static_cast<int>(DHT11), dht11Domoticz);
              Serial.println("DHT11 sensor added!");
          }
          else if (cfg.type == DHT22) {
              DomoticzData_t dht22Domoticz;
              dht22Domoticz.m_idx = cfg.domoticz_idx;
              dht22Domoticz.setpointIdx = cfg.domoticz_setpoint_idx;
              sensor[i] = new DhtSensor(&wifiClient, cfg.pin, static_cast<int>(DHT22), dht22Domoticz);
              Serial.println("DHT22 sensor added!");
          }
          /*else if (cfg.type == DS18B20) {
              sensor[i] = new Ds18b20Sensor(&wifiClient, cfg.pin, cfg.domoticz_idx, cfg.domoticz_setpoint_idx);
              Serial.println("DS18B20 sensor added!");
          }*/
          else if (cfg.type == MULTIDS18) {
              // TODO: add correct parameters
              sensor[i] = new MultiDs18b20Sensor(&wifiClient, cfg.pin);
              Serial.println("MULTIDS18 sensor added!");
          }
      }
      Serial.println("ESP Configured from json!");
      display.clearDisplay();
      display.setTextSize(1);
      display.setTextColor(WHITE);
      display.setCursor(0, 0);
      display.println("config.json OK");
      display.printf("%u sensors added!\n", config->getNoOfSensors());
      display.display();

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
        display.clearDisplay();
        display.setCursor(0, 0);
        for (uint8_t i = 0; i < config->getNoOfSensors(); i++)
        {
            sensor[i]->read();
            sensor[i]->updateData();
            sensor[i]->getSetPointVal();
            display.printf("%s\n", sensor[i]->data().c_str());            
            /*Serial.printf("Free read: %u\n", ESP.getFreeHeap());
            Serial.printf("Free read: %4d\n", cont_get_free_stack(&g_cont));*/
        }
        display.display();
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
