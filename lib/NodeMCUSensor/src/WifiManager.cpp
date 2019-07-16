/**
 * Project NodeMCUSensor
 */


#include "WifiManager.h"
#include "WiFiManager.h"

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
WifiManager::WifiManager(std::vector <WiFiManagerParameter> &wifiParameters)
{
  // With parameters, don't reset settings
  WiFiManager wifiManager;
  
  Serial.println("Loading WifiManagerParameters...");
  for (std::vector<WiFiManagerParameter>::size_type a = 0; 
       a != wifiParameters.size(); 
       a++)
  {
    WiFiManagerParameter *parm = new WiFiManagerParameter(
      wifiParameters[a].getID(),
      wifiParameters[a].getPlaceholder(),
      wifiParameters[a].getValue(),
      wifiParameters[a].getValueLength(),
      wifiParameters[a].getCustomHTML());
    wifiManager.addParameter(parm);
  }
  Serial.println("Done loading WifiManagerParameters...");
  
  // set callback that gets called when connecting to previous 
  //   WiFi fails, and enters Access Point mode
  wifiManager.setAPCallback(WifiManager::configModeCallback);
  wifiManager.setSaveConfigCallback(WifiManager::saveConfigCallback);  
  
  // fetches ssid and pass and tries to connect
  //   if it does not connect it starts an access point with the specified name
  //   and goes into a blocking loop awaiting configuration
  Serial.println("Starting WifiManager autoConnect()...");
  if (!wifiManager.autoConnect()) {
    Serial.println("failed to connect and hit timeout");
    //reset and try again, or maybe put it to deep sleep
    ESP.reset();
    delay(1000);
  }
  Serial.println("Finished WifiManager autoConnect()...");
  
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
WifiManager::WifiManager(std::vector <WiFiManagerParameter> &wifiParameters,
                         bool resetSettings)
{
  // With parameters, reset settings
  WiFiManager wifiManager;
  
  if (resetSettings)
  {
    wifiManager.resetSettings();
  }

  for (std::vector<WiFiManagerParameter>::size_type a = 0; 
       a != wifiParameters.size(); 
       a++)
  {
    WiFiManagerParameter *parm = new WiFiManagerParameter(
      wifiParameters[a].getID(),
      wifiParameters[a].getPlaceholder(),
      wifiParameters[a].getValue(),
      wifiParameters[a].getValueLength(),
      wifiParameters[a].getCustomHTML());
    wifiManager.addParameter(parm);
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

void WifiManager::saveConfig(std::vector <WiFiManagerParameter> &wifiParameters)
{
  Filesystem fs;
  
  Serial.println("saving config to fs");
  
  for (unsigned int a = 0;
       a != wifiParameters.size(); 
       ++a)
  {
    fs.updateKey(wifiParameters[a].getID(), 
                 wifiParameters[a].getValue());
  }
  
  fs.flushToFs();
}

/**
 * @return bool
 */
bool WifiManager::connectToWifi()
{
  return false;
}

void WifiManager::loadFromFS(std::vector <WiFiManagerParameter> &wifiParameters)
{
  Filesystem fs;
  for (unsigned int i=0; i < wifiParameters.size(); ++i)
  {
    std::string savedValue = fs.loadFromFs(wifiParameters[0].getID()); 
    if (savedValue != "")
    {
      WiFiManagerParameter *newParam = new 
        WiFiManagerParameter(wifiParameters[i].getID(),
                             savedValue.c_str(),
                             savedValue.c_str(),
                             savedValue.size()+1);
      wifiParameters[i] = *newParam;
    }
  }
}
