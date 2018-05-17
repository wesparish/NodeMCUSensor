download:
	mkdir -p libs tools
	[ -d libs/NtpClient ]       || git clone https://github.com/gmag11/NtpClient.git libs/NtpClient
	[ -d libs/ArduinoJson ]     || git clone https://github.com/bblanchon/ArduinoJson.git libs/ArduinoJson
	[ -d libs/Time ]            || git clone https://github.com/PaulStoffregen/Time libs/Time
	[ -d libs/WiFiManager ]     || git clone https://github.com/wesparish/WiFiManager.git libs/WiFiManager
  # Tools
	[ -d tools/makeEspArduino ] || git clone https://github.com/wesparish/makeEspArduino.git tools/makeEspArduino
	[ -d tools/esp8266 ]        || git clone https://github.com/wesparish/Arduino.git tools/esp8266 && cd tools/esp8266/tools && python get.py
	# Test Framework
	[ -d test/googletest ]      || git clone https://github.com/google/googletest.git test/googletest && cd test/googletest/googletest/make && make


CLEAN_DIRS = libs tools

# My makefile
SKETCH = ./src/TempSensorMain.ino


-include Makefile.common

#
# UPLOAD_PORT = /dev/ttyUSB1
# BOARD = esp210
#

-include tools/makeEspArduino/makeEspArduino.mk
