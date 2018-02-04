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
#include "oneWireSensor.h"
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
      uint8_t sensorIdx;
      
      for (sensorIdx = 0; sensorIdx < config->getNoOfDhtSensors(); sensorIdx++)
      {
          auto cfg = config->dhtConfig[sensorIdx];
          DomoticzData_t domoticzData;
          domoticzData.m_idx = cfg.domoticzCfg.idx;
          domoticzData.setpointIdx = cfg.domoticzCfg.setpoint_idx;
          if (cfg.sensor.type == DHT11) {              
              sensor[sensorIdx] = new DhtSensor(&wifiClient, cfg.sensor.pin, static_cast<int>(DHT11), domoticzData);
              Serial.println("DHT11 sensor added!");
          }
          else if (cfg.sensor.type == DHT22) {
              sensor[sensorIdx] = new DhtSensor(&wifiClient, cfg.sensor.pin, static_cast<int>(DHT22), domoticzData);
              Serial.println("DHT22 sensor added!");
          }          
      }
      auto cfg = config->oneWireConfig;
      if (cfg.sensor.type == DS18B20) {
          sensor[sensorIdx] = new OneWireSensor(&wifiClient, cfg.sensor.pin);
          OneWireSensor* ds18b20 = (OneWireSensor*)sensor[sensorIdx];
          for (uint8_t i = 0; i < config->getNoOfDallasTemperatureSensors(); i++)
          {
              Ds18B20Sensor_t* dsSensor = new Ds18B20Sensor_t();
              dsSensor->domoticzData.m_idx = cfg.domoticzCfg[i].idx;
              ds18b20->addDs18B20Sensor(dsSensor, i);
          }
          Serial.println("DS18B20 sensor(s) added!");
          ds18b20->resolve();
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
  else {
      Serial.println("mDNS responder started");
  }

  wifiServer->start();
  Serial.println("espWifiServer started");
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
            Serial.println("Sensor read OK");
            sensor[i]->updateData();
            Serial.println("SensorData updated");
            //sensor[i]->getSetPointVal();
            //Serial.println("SetPoint data fetched");
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
