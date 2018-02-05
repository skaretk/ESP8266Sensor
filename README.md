# ESP8266Sensor
Generic ESP8266 Generic sensor for open source home automation system [Domoticz](www.domoticz.com)

## Current features

Feature | Arduino | NodeMCU
------- | ------ | -------
Configurable | **Y**   | **N**
AccessPoint  | **Y**   | **N**
Domoticz     | **Y**   | **Y**
DNS          | **Y**   | **N**
DHT11/DHT22  | **Y(4x)**   | **Y**
DS18B20      | **Y(4x on one OneWire Bus)**   | **N**
SSD1306      | **Dev** | **N**

## Arduino:
Includes Arduino sketches

In order to build the Arduino SW package, all of the listed libraries must be installed
1. Install The ESP8266 Arduino core
2. Install all libraries listed
3. Arduino ESP8266 File System Uploaded must be added manually, refer to instructions

Required Libraries
* [ESP8266/Arduino](https://github.com/esp8266/Arduino)
* [OneWire](https://www.pjrc.com/teensy/td_libs_OneWire.html)
* [DallasTemperature - DS18B20](https://github.com/milesburton/Arduino-Temperature-Control-Library)
* [DHT sensor library](https://github.com/adafruit/DHT-sensor-library)
* [ArduinoJson](https://arduinojson.org/)
* [LCD - SSD1306](https://github.com/adafruit/Adafruit_SSD1306)
* [LCD - Adafruit GFX Library](https://github.com/adafruit/Adafruit-GFX-Library)
* [Arduino ESP8266 File System Uploader](https://github.com/esp8266/arduino-esp8266fs-plugin)

### Config
The SW package requres that the data folder has been flashed into the SPIFFS file system. This can be done with the File System Uploader
File | Info
---- | ---
config.json  | Main Configuration file
config.html  | Basic HTML form for uploading new config
success.html | Indication when upload has been executed

When bootup, the software searches for 'config.json' stored in the SPIFFS on the ESP8266
config.json does not need to be flashed with the Upload tool, but can be updated from a web browser:
1. Connect to ESP8266 WIFI AP
2. Open ip/config in browser
3. Select config.json file and upload

### Config file

```javascript
{
	"wifi": {
		"ssid": "SSID",
		"password": "PW",
		"ip": "x.x.x.x",                // Domoticz IP Address
		"port": "PORT",                 // Domoticz Port
		"dns": "esptester"              // DNS to be used
	},
	"sensors": [{
			"pin": "0",                   // Pin where DHT sensor is connected
			"type": "DHT11",              // Sensor Type
			"idx": "0",                   // Domoticz Index for DHT temp/humidity sensor
			"setpointIdx": "1"            // Optional Setpoint
		}, {
			"pin": "1",                   // OneWire Pin
			"type": "OneWire",            // Type of sensor
			"ds18b20sensors": [{          // ds18b20Sensor array
					"idx": "2"                // Domoticz Index for DS18B20 temp sensor
				}
			]
		}
	],
	"sleep": {
		"deepSleep": "false",           // Deepsleep, requires extra cable so ESP8266 will wake up
		"sleepTime": "60"               // sleep/waitime between readings in Seconds
	},
	"led": {
		"enabled": "false"              // Not yet implemented
	}
}
```
## Lua:
This SW package only includes one DHT sensor, and is not maintained any longer

Required packages:
1. ESP8266 Flasher
2. ESPlorer

Fritzing Software:
Drawings of the hardware used
