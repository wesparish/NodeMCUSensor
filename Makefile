# My makefile
SKETCH = ./src/TempSensorMain.ino

ESP_ROOT=/home/wes/esp8266

INCLUDE_DIRS += ./include
INCLUDE_DIRS += ./libs/Time/
INCLUDE_DIRS += ./libs/NtpClient/src/
INCLUDE_DIRS += $(ESP_ROOT)/libraries/ESP8266WiFi/src/
INCLUDE_DIRS += $(ESP_ROOT)/libraries/ESP8266HTTPClient/src/
INCLUDE_DIRS += libs/ArduinoJson/src/

LIBS += ./libs/Time
LIBS += ./libs/NtpClient/src/ 

VERBOSE=1
BUILD_ROOT = ./build/
#
# UPLOAD_PORT = /dev/ttyUSB1
# BOARD = esp210
#
include $(HOME)/makeEspArduino/makeEspArduino.mk
