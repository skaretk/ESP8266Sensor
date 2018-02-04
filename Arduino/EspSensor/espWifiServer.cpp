#include "espWifiServer.h"

void EspWifiServer::start()
{
    if (!m_started) {
        server.on("/", std::bind(&EspWifiServer::handleRoot, this));
        server.on("/dir", HTTP_GET, std::bind(&EspWifiServer::handleDir, this));
        server.on("/config", HTTP_GET, std::bind(&EspWifiServer::handleConfigGET, this));
        server.on("/config", HTTP_POST, std::bind(&EspWifiServer::handleConfigPOST, this), std::bind(&EspWifiServer::handleFileUpload, this));
        server.onNotFound(std::bind(&EspWifiServer::handleNotFound, this));
        server.begin();
        Serial.println("HTTP server started");
    }
    else {
        Serial.println("HTTP server already started");
    }
}

void EspWifiServer::handle()
{
    server.handleClient();
}

void EspWifiServer::handleRoot()
{
    Serial.println("\n[Client connected]");
    if (m_cfg->wifiConfig.configured)
        server.send(200, "text/html", "ESP Configured!");
    else
        server.send(200, "text/html", "ESP NOT Configured, please upload configuration - /upload");
    Serial.println("[Client disonnected]");
}

void EspWifiServer::handleDir()
{
    Dir dir = SPIFFS.openDir("/");

    String output = "[";
    while (dir.next()) {
        File entry = dir.openFile("r");
        if (output != "[") output += ',';
        bool isDir = false;
        output += "{\"type\":\"";
        output += (isDir) ? "dir" : "file";
        output += "\",\"name\":\"";
        output += String(entry.name()).substring(1);
        output += "\"}";
        entry.close();
    }

    output += "]";
    server.send(200, "application/json", output);
}

void EspWifiServer::handleConfigGET()
{
    if (!handleFileRead("/config.html"))                    // send it if it exists
        server.send(404, "text/plain", "404: Not Found");   // otherwise, respond with a 404 (Not Found) error
}

void EspWifiServer::handleConfigPOST()
{
    server.send(200);       // Send status 200 (OK) to tell the client we are ready to receive
}

void EspWifiServer::handleNotFound()
{
    if (!handleFileRead(server.uri()))                      // send it if it exists
        server.send(404, "text/plain", "404: Not Found");   // otherwise, respond with a 404 (Not Found) error
}

String EspWifiServer::getContentType(String filename) {     // convert the file extension to the MIME type
    if (filename.endsWith(".html")) return "text/html";
    else if (filename.endsWith(".json")) return "application/json";
    return "text/plain";
}

bool EspWifiServer::handleFileRead(String path) {           // send the right file to the client (if it exists)
    Serial.println("handleFileRead: " + path);
    if (path.endsWith("/")) path += "index.html";           // If a folder is requested, send the index file
    String contentType = getContentType(path);              // Get the MIME type
    if (SPIFFS.exists(path)) {                              // If the file exists, either as a compressed archive, or normal
        File file = SPIFFS.open(path, "r");                 // Open the file
        size_t sent = server.streamFile(file, contentType); // Send it to the client
        file.close();                                       // Close the file again
        Serial.println(String("\tSent file: ") + path);
        return true;
    }
    Serial.println(String("\tFile Not Found: ") + path);   // Return false if files does not exist
    return false;
}

void EspWifiServer::handleFileUpload() {                    // upload a new file to the SPIFFS
    HTTPUpload& upload = server.upload();

    switch (upload.status) {
        case UPLOAD_FILE_START: {
            String filename = upload.filename;
            if (!filename.startsWith("/")) filename = "/" + filename;
            Serial.print("handleFileUpload Name: "); Serial.println(filename);
            fsUploadFile = SPIFFS.open(filename, "w");              // Open the file for writing in SPIFFS (create if it doesn't exist)
            filename = String();
            break;
        }
        case UPLOAD_FILE_WRITE:
            if (fsUploadFile)
                fsUploadFile.write(upload.buf, upload.currentSize); // Write the received bytes to the file
            break;
        case UPLOAD_FILE_END:
            if (fsUploadFile) {                                     // If the file was successfully created
                fsUploadFile.close();                               // Close the file again
                Serial.print("handleFileUpload Size: "); Serial.println(upload.totalSize);
                server.sendHeader("Location", "/success.html");     // Redirect the client to the success page
                server.send(303);
            }
            else {
                server.send(500, "text/plain", "500: couldn't create file");
            }
            break;
        default:
            server.send(500, "text/plain", "500: couldn't create file");
    }
}