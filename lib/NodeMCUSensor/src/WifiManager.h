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
  
  bool loadFromFS(std::vector <WiFiManagerParameter*> &wifiParameters);
  void saveConfig(std::vector <WiFiManagerParameter*> &wifiParameters);

protected:
  static void configModeCallback(WiFiManager *myWiFiManager);
  static void saveConfigCallback();
  
private:
  // Common to all ctors
  void wmSetup(WiFiManager&, bool startCaptivePortal);
  // Common to parameter ctors
  bool parameterSetup(WiFiManager&, std::vector<WiFiManagerParameter*>&);
  static bool _shouldSaveConfig;
};

#endif //_WIFIMANAGER_H
