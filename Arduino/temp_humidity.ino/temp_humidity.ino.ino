#include <ESP8266WiFi.h>
#include <DHT.h>

// #include "domoticzCfg.h"
#include "wifiClient.h"
#include "Config.h"

// Temperature and humidity
float temp = 0;
float humidity = 0;
float hic = 0; // heat index

ESPConfig espConfig;
WifiClient wifiClient;

// Initialize DHT sensor.
// Note that older versions of this library took an optional third parameter to
// tweak the timings for faster processors.  This parameter is no longer needed
// as the current DHT reading algorithm adjusts itself to work on faster procs.
DHT dht(espConfig.dhtPin, espConfig.dhtType);

void setup()
{
  Serial.begin(115200);
  Serial.setTimeout(2000);
  // delay(10);
  // Wait for serial to initialize.
  while(!Serial) { }
  
  Serial.println("ESP8266 Initliazing!");   
  wifiClient.connect();  
  dht.begin();
}

void loop()
{
    // Get readings from all sensors
    readDht();
    //printInfo();
    wifiClient.updateData(temp, humidity);
    if (espConfig.sleep.enabled) {
      Serial.println("Deepsleep enabled");
      ESP.deepSleep(espConfig.sleep.sleepTimeS * 1000000);
    }
    else {
      Serial.println("Deepsleep not enabled");  
      delay(espConfig.sleep.sleepTimeS * 1000); // Wait 60 seconds
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

