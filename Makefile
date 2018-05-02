download:
	mkdir -p libs tools
	[ -d libs/NtpClient ]       || git clone https://github.com/gmag11/NtpClient.git libs/NtpClient
	[ -d libs/ArduinoJson ]     || git clone https://github.com/bblanchon/ArduinoJson.git libs/ArduinoJson
	[ -d libs/Time ]            || git clone https://github.com/PaulStoffregen/Time libs/Time
	[ -d libs/WiFiManager ]     || git clone https://github.com/wesparish/WiFiManager.git libs/WiFiManager
  # Tools
	[ -d tools/makeEspArduino ] || git clone https://github.com/wesparish/makeEspArduino.git tools/makeEspArduino
	[ -d tools/esp8266 ]        || git clone https://github.com/wesparish/Arduino.git tools/esp8266 && cd tools/esp8266/tools && python get.py


CLEAN_DIRS = libs tools

# My makefile
SKETCH = ./src/TempSensorMain.ino

ESP_ROOT=tools/esp8266

INCLUDE_DIRS += ./include
INCLUDE_DIRS += ./libs/Time/
INCLUDE_DIRS += ./libs/NtpClient/src/
INCLUDE_DIRS += ./libs/WiFiManager/
INCLUDE_DIRS += $(ESP_ROOT)/libraries/ESP8266WiFi/src/
INCLUDE_DIRS += $(ESP_ROOT)/libraries/ESP8266HTTPClient/src/
INCLUDE_DIRS += $(ESP_ROOT)/libraries/DNSServer/src/
INCLUDE_DIRS += $(ESP_ROOT)/libraries/ESP8266WebServer/src/
INCLUDE_DIRS += libs/ArduinoJson/src/

LIBS += ./libs/Time
LIBS += ./libs/NtpClient/src/
LIBS += ./libs/ArduinoJson/src/
LIBS += ./libs/WiFiManager/
LIBS += $(ESP_ROOT)/libraries/ESP8266HTTPClient/src/
LIBS += $(ESP_ROOT)/libraries/ESP8266WiFi/src/
LIBS += $(ESP_ROOT)/libraries/ESP8266WebServer/
LIBS += $(ESP_ROOT)/libraries/DNSServer/

#VERBOSE=1
BUILD_ROOT = ./build/
#
# UPLOAD_PORT = /dev/ttyUSB1
# BOARD = esp210
#

-include tools/makeEspArduino/makeEspArduino.mk
