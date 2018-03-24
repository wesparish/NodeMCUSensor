# My makefile
SKETCH = ./src/TempSensorMain.ino

ESP_ROOT=/home/wes/esp8266

INCLUDE_DIRS += ./include
VERBOSE=1
BUILD_ROOT = ./build/
#
# UPLOAD_PORT = /dev/ttyUSB1
# BOARD = esp210
#
include $(HOME)/makeEspArduino/makeEspArduino.mk
