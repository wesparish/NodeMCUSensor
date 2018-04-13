/**
 * Project NodeMCUSensor
 */


#ifndef _WIFIMANAGER_H
#define _WIFIMANAGER_H

#include <FS.h>
#include "DNSServer.h"
#include "ESP8266WebServer.h"
#include "WiFiManager.h"
#include "ArduinoJson.h"

class WifiManager {
public:
  WifiManager();
  WifiManager(std::vector <WiFiManagerParameter> wifiParameters,
              bool resetWifiSettings);
  WifiManager(std::vector <WiFiManagerParameter> wifiParameters);
  
  bool connectToWifi();
  
protected:
  void configModeCallback(WiFiManager *myWiFiManager);
  void loadFromFS();
  
private:
  std::vector <WiFiManagerParameter> _wifiParameters;
  //JsonObject _jsonConfigFileData;
};

#endif //_WIFIMANAGER_H
