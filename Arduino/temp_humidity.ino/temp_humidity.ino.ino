#include <ESP8266WiFi.h>
#include <DHT.h>

#include "config.h"
#include "wifiClient.h"

#include <DallasTemperature.h>
#include <OneWire.h>

// Dallas
#define ONE_WIRE_BUS 12
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature dallasTemp(&oneWire);

// Temperature and humidity
float temp = 0;
float humidity = 0;
float hic = 0; // heat index

SleepCfg sleepCfg;
DhtCfg dhtCfg;
WifiClient wifiClient;
Config config;

// Initialize DHT sensor.
// Note that older versions of this library took an optional third parameter to
// tweak the timings for faster processors.  This parameter is no longer needed
// as the current DHT reading algorithm adjusts itself to work on faster procs.
DHT dht(dhtCfg.pin, dhtCfg.type);

void setup()
{
  Serial.begin(74880);
  delay(10);
  // Wait for serial to initialize.
  while(!Serial) { }
  if (config.readJson()) {
      Serial.println("ESP Configured from json");
      wifiClient.getWifiConfig(config.getWifi());
  }
  
  Serial.println("ESP8266 Initliazing!");   
  wifiClient.connect();  
  dht.begin();
  dallasTemp.begin();
}

void loop()
{
    // Get readings from all sensors
    readDht();
    readDallas();
    //printInfo();
    wifiClient.updateData(temp, humidity);
    if (sleepCfg.deepSleep) {
      Serial.println("Deepsleep enabled");
      ESP.deepSleep(sleepCfg.sleepTime * 1000000);
    }
    else {
      Serial.println("Deepsleep not enabled");  
      delay(sleepCfg.sleepTime * 1000); // Wait 60 seconds
    }
}

void readDht()
{
  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  humidity = dht.readHumidity();
  // Read temperature as Celsius (the default)
  temp = dht.readTemperature();

  // Check if any reads failed and exit early (to try again).
  if (isnan(humidity) || isnan(temp)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  // Compute heat index in Celsius (isFahreheit = false)
  hic = dht.computeHeatIndex(temp, humidity, false);

  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.print(" %\t");
  Serial.print("Temperature: ");
  Serial.print(temp);
  Serial.print(" *C ");
  Serial.print("Heat index: ");
  Serial.print(hic);
  Serial.print(" *C ");
  Serial.println("");
}

void readDallas()
{
    Serial.print(" Requesting dallas temp...");
    dallasTemp.requestTemperatures(); // Send the command to get temperature readings 
    Serial.println("DONE");
    Serial.print("Temperature is: ");
    Serial.print(dallasTemp.getTempCByIndex(0));
    // Why "byIndex"?  You can have more than one DS18B20 on the same bus. 0 refers to the first IC on the wire 
}


