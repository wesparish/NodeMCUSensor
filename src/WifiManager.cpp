/**
 * Project NodeMCUSensor
 */


#include "WifiManager.h"

bool WifiManager::_shouldSaveConfig = false;

/**
 * WifiManager implementation
 * 
 * Wifi management class
 */

/**
 * Constructor
 */
WifiManager::WifiManager()
{
  // No parameters, don't reset settings
  // Local intialization. Once its business is done,
  //   there is no need to keep it around
  WiFiManager wifiManager;
  
  // set callback that gets called when connecting to previous 
  //   WiFi fails, and enters Access Point mode
  wifiManager.setAPCallback(WifiManager::configModeCallback);
  wifiManager.setSaveConfigCallback(WifiManager::saveConfigCallback);  
  
  // fetches ssid and pass and tries to connect
  //   if it does not connect it starts an access point with the specified name
  //   and goes into a blocking loop awaiting configuration
  if (!wifiManager.autoConnect()) {
    Serial.println("failed to connect and hit timeout");
    //reset and try again, or maybe put it to deep sleep
    ESP.reset();
    delay(1000);
  }
}

/**
 * Constructor with parameter: 
 * vector - configuration parameters
 */
WifiManager::WifiManager(std::vector <WiFiManagerParameter> wifiParameters)
{
  // With parameters, don't reset settings
  // Local intialization. Once its business is done,
  //   there is no need to keep it around
  WiFiManager wifiManager;
  
  for (std::vector<WiFiManagerParameter>::size_type a = 0; 
       a != wifiParameters.size(); 
       a++)
  {
    wifiManager.addParameter(&wifiParameters[a]);
  }
  
  // set callback that gets called when connecting to previous 
  //   WiFi fails, and enters Access Point mode
  wifiManager.setAPCallback(WifiManager::configModeCallback);
  wifiManager.setSaveConfigCallback(WifiManager::saveConfigCallback);  
  
  // fetches ssid and pass and tries to connect
  //   if it does not connect it starts an access point with the specified name
  //   and goes into a blocking loop awaiting configuration
  if (!wifiManager.autoConnect()) {
    Serial.println("failed to connect and hit timeout");
    //reset and try again, or maybe put it to deep sleep
    ESP.reset();
    delay(1000);
  }
  
  if (WifiManager::_shouldSaveConfig)
  {
    saveConfig(wifiParameters);
  }
}

/**
 * Constructor with parameter: 
 * vector - configuration parameters
 * bool - reset settings
 */
WifiManager::WifiManager(std::vector <WiFiManagerParameter> wifiParameters,
                         bool resetSettings)
{
  // With parameters, reset settings
  // Local intialization. Once its business is done,
  //   there is no need to keep it around
  WiFiManager wifiManager;
  
  wifiManager.resetSettings();
  
  for (std::vector<WiFiManagerParameter>::size_type a = 0; 
       a != wifiParameters.size(); 
       a++)
  {
    wifiManager.addParameter(&wifiParameters[a]);
  }
  
  // set callback that gets called when connecting to previous 
  //   WiFi fails, and enters Access Point mode
  wifiManager.setAPCallback(WifiManager::configModeCallback);
  wifiManager.setSaveConfigCallback(WifiManager::saveConfigCallback);  
  
  // fetches ssid and pass and tries to connect
  //   if it does not connect it starts an access point with the specified name
  //   and goes into a blocking loop awaiting configuration
  if (!wifiManager.autoConnect()) {
    Serial.println("failed to connect and hit timeout");
    //reset and try again, or maybe put it to deep sleep
    ESP.reset();
    delay(1000);
  }
  
  if (WifiManager::_shouldSaveConfig)
  {
    saveConfig(wifiParameters);
  }
}

// gets called when WiFiManager enters configuration mode
void WifiManager::configModeCallback (WiFiManager *myWiFiManager) {
  Serial.println("Entered config mode");
  Serial.println(WiFi.softAPIP());
  // if you used auto generated SSID, print it
  Serial.println(myWiFiManager->getConfigPortalSSID());
  // entered config mode, make led toggle faster
  //ticker.attach(0.2, tick);
}

// callback notifying us of the need to save config
void WifiManager::saveConfigCallback () {
  Serial.println("Should save config");
  WifiManager::_shouldSaveConfig = true;
}

void saveConfig(std::vector <WiFiManagerParameter> &wifiParameters)
{
  Serial.println("saving config to fs");
  DynamicJsonBuffer jsonBuffer;
  JsonObject& json = jsonBuffer.createObject();
  
  for (int a = 0; 
       a != wifiParameters.size(); 
       ++a)
  {
    json[wifiParameters[a].getID()] = wifiParameters[a].getValue();
  }
  
  File configFile = SPIFFS.open("/config.json", "w");
  if (!configFile) {
    Serial.println("failed to open config file for writing");
  }

  json.printTo(Serial);
  json.printTo(configFile);
  configFile.close();
}

/**
 * @return bool
 */
bool WifiManager::connectToWifi()
{
  return false;
}

void WifiManager::loadFromFS()
{
}