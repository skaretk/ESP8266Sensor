#pragma once
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <FS.h>   // Include the SPIFFS library
#include "config.h"

class EspWifiServer
{
public:
    EspWifiServer(Config* cfg) : m_cfg(cfg), server(80) { };
    void start();
    void handle();

    File fsUploadFile;              // a File object to temporarily store the received file
    String getContentType(String filename); // convert the file extension to the MIME type
    bool handleFileRead(String path);       // send the right file to the client (if it exists)
    void handleFileUpload();                // upload a new file to the SPIFFS

private:
    ESP8266WebServer server;
    Config* m_cfg;
    void handleRoot();
    void handleConfigGET();
    void handleConfigPOST();
    void handleDir();
    void handleNotFound();
    
    bool m_started{ false };
};