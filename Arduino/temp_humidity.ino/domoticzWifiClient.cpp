#include <ESP8266WiFi.h>
#include "domoticzWifiClient.h"

void DomoticzWifiClient::setWifiConfig(WifiCfg* cfg)
{
    ssid = cfg->ssid;
    password = cfg->password;
    domoticzIp = cfg->domoticz_ip;
    domoticzPort = cfg->domoticz_port;
    cfg->configured = true;
}

void DomoticzWifiClient::connect()
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

void DomoticzWifiClient::updateData(float temperature, float humidity, int idx)
{
    // Use WiFiClient class to create TCP connections
    WiFiClient client;

    // Domoticz format /json.htm?type=command&param=udevice&idx=IDX&nvalue=0&svalue=TEMP;HUM;HUM_STAT

    if (client.connect(domoticzIp, domoticzPort)) {  
        client.print("GET /json.htm?type=command&param=udevice&idx=");
        client.print(idx);
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
    else {
        Serial.println("Failed to connect!");
    }
}

void DomoticzWifiClient::updateData(float temperature, int idx)
{
    // Use WiFiClient class to create TCP connections
    WiFiClient client;

    // Domoticz format /json.htm?type=command&param=udevice&idx=IDX&nvalue=0&svalue=TEMP

    if (client.connect(domoticzIp, domoticzPort)) {
        client.print("GET /json.htm?type=command&param=udevice&idx=");
        client.print(idx);
        client.print("&nvalue=0&svalue=");
        client.print(temperature);
        client.println(" HTTP/1.1");
        client.print("Host: ");
        client.print(domoticzIp);
        client.print(":");
        client.println(domoticzPort);
        client.println("Connection: close");
        client.println();

        client.stop();
    }
    else {
        Serial.println("Failed to connect!");
    }
}

void DomoticzWifiClient::getJson(int idx , String& json)
{
    // Use WiFiClient class to create TCP connections
    WiFiClient client;
    client.setTimeout(2000);

    // Domoticz format /json.htm?type=devices&rid=IDX

    Serial.println("Client.connect()");

    if (client.connect(domoticzIp, domoticzPort)) {
        Serial.println("http GET");
        client.print("GET /json.htm?type=devices&rid=");
        client.print(idx);
        client.println(" HTTP/1.1");
        client.print("Host: ");
        client.print(domoticzIp);
        client.print(":");
        client.println(domoticzPort);
        client.println("Connection: close");
        client.println();
        Serial.println("http GET OK");

        boolean hdrFinished = false;        
        while (client.connected())
        {
            //Serial.println("Client connected");
            if (client.available())
            {
                //Serial.println("Client available");
                if (!hdrFinished) {
                    String line = client.readStringUntil('\r');
                    Serial.print(line);
                    // wait for end of client's request, that is marked with an empty line
                    if (line.length() == 1 && line[0] == '\n') {
                        hdrFinished = true;
                    }             
                }
                else { // Get json
                    json += client.readStringUntil('\n');
                }
            }            
        }
        Serial.println("Client not available");
        client.stop();
        Serial.println("Client stopped");
    }
    else {
        Serial.println("Failed to connect!");
    }    
}

const char* DomoticzWifiClient::getJsonResultData(int idx)
{
    /*Serial.printf("Free before: %u\n", ESP.getFreeHeap());
    Serial.printf("Free before: %4d\n", cont_get_free_stack(&g_cont));*/
    String json = "";
    getJson(idx, json);
    /*Serial.printf("Free after: %u\n", ESP.getFreeHeap());
    Serial.printf("Free after: %4d\n", cont_get_free_stack(&g_cont));*/
    Serial.println(json);
    StaticJsonBuffer<1500> jsonBuffer;
    JsonObject& root = jsonBuffer.parseObject(json);
    // Test if parsing succeeds.
    if (!root.success()) {
        Serial.println("getJsonResultData - parseObject() failed");
        return "";
    }
    Serial.println("parseObject() OK");
    JsonArray& results = root["result"];
    for (auto& result : results) {
        const char* data = result["Data"];
        return data;
    }
    return "";
}

