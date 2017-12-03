#include <ESP8266WiFi.h>
#include "wifiClient.h"

void WifiClient::getWifiConfig(WifiCfg cfg)
{
    ssid = cfg.ssid;
    password = cfg.password;
    domoticzIp = cfg.domoticz_ip;
    domoticzPort = cfg.domoticz_port;
    domoticzIdx = cfg.domoticz_idx;
    configured = true;
}

void WifiClient::connect()
{
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);
  
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

  if (client.connect(domoticzIp, domoticzPort)) {  
    client.print("GET /json.htm?type=command&param=udevice&idx=");
    client.print(domoticzIdx);
    client.print("&nvalue=0&svalue=");
    client.print(temperature);
    client.print(";");
    client.print(humidity);
    client.print(";0"); //Value for HUM_STAT. Can be one of: 0=Normal, 1=Comfortable, 2=Dry, 3=Wet
    client.println(" HTTP/1.1");
    client.print("Host: ");
    client.print(domoticzIp);
    client.print(":");
    client.println(domoticzPort);
    client.println("Connection: close");
    client.println();

    client.stop();
  }
}
