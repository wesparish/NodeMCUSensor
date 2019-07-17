/**
 * Project NodeMCUSensor
 */


#ifndef _WIFIMANAGER_H
#define _WIFIMANAGER_H

#include "Filesystem.h"

#include <FS.h>
#include "DNSServer.h"
#include "ESP8266WebServer.h"
#include "WiFiManager.h"
#include "ArduinoJson.h"

class WifiManager {
public:
  WifiManager();
  WifiManager(std::vector <WiFiManagerParameter*> &wifiParameters,
              bool resetWifiSettings);
  WifiManager(std::vector <WiFiManagerParameter*> &wifiParameters);
  
  void loadFromFS(std::vector <WiFiManagerParameter*> &wifiParameters);
protected:
  static void configModeCallback(WiFiManager *myWiFiManager);
  void saveConfig(std::vector <WiFiManagerParameter*> &wifiParameters);
  static void saveConfigCallback();
  
private:
  static bool _shouldSaveConfig;
};

#endif //_WIFIMANAGER_H
