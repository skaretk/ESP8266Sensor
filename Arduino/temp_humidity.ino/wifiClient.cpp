#include <ESP8266WiFi.h>
#include "wifiClient.h"

void WifiClient::connect()
{
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(wifiCfg.ssid);
  
  WiFi.begin(wifiCfg.ssid, wifiCfg.password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void WifiClient::updateData(float temperature, float humidity)
{
  // Use WiFiClient class to create TCP connections
  WiFiClient client;
  
  // Domoticz format /json.htm?type=command&param=udevice&idx=IDX&nvalue=0&svalue=TEMP;HUM;HUM_STAT

  if (client.connect(domoticz.ip, domoticz.port)) {  
    client.print("GET /json.htm?type=command&param=udevice&idx=");
    client.print(domoticz.idx);
    client.print("&nvalue=0&svalue=");
    client.print(temperature);
    client.print(";");
    client.print(humidity);
    client.print(";0"); //Value for HUM_STAT. Can be one of: 0=Normal, 1=Comfortable, 2=Dry, 3=Wet
    client.println(" HTTP/1.1");
    client.print("Host: ");
    client.print(domoticz.ip);
    client.print(":");
    client.println(domoticz.port);
    client.println("Connection: close");
    client.println();

    client.stop();
  }
}
