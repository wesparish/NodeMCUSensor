/**
 * Project NodeMCUSensor
 */


#include "WifiManager.h"

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
    
}

/**
 * Constructor with parameter: 
 * vector - configuration parameters
 */
WifiManager::WifiManager(std::vector <WiFiManagerParameter> wifiParameters)
{
  
}

/**
 * Constructor with parameter: 
 * vector - configuration parameters
 * bool - reset settings
 */
WifiManager::WifiManager(std::vector <WiFiManagerParameter> wifiParameters,
                         bool resetSettings)
{
  
}


/**
 * @return bool
 */
bool WifiManager::connectToWifi()
{
  return false;
}

//gets called when WiFiManager enters configuration mode
void WifiManager::configModeCallback (WiFiManager *myWiFiManager)
{
  Serial.println("Entered config mode");
  Serial.println(WiFi.softAPIP());
  //if you used auto generated SSID, print it
  Serial.println(myWiFiManager->getConfigPortalSSID());
  //entered config mode, make led toggle faster
  //ticker.attach(0.2, tick);
}

void WifiManager::loadFromFS()
{
}