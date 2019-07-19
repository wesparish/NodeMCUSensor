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
  wmSetup(wifiManager, false);
}

/**
 * Constructor with parameter: 
 * vector - configuration parameters
 */
WifiManager::WifiManager(std::vector <WiFiManagerParameter*> &wifiParameters)
{
  // With parameters, don't reset settings
  WiFiManager wifiManager;
  bool loadedFromFS = parameterSetup(wifiManager, wifiParameters);
  // Force captive protal if parms not loaded from FS
  wmSetup(wifiManager, !loadedFromFS);
  
  if (WifiManager::_shouldSaveConfig)
  {
    Serial.println("Saving Wifi config...");
    saveConfig(wifiParameters);
  }
}

/**
 * Constructor with parameter: 
 * vector - configuration parameters
 * bool - reset settings
 */
WifiManager::WifiManager(std::vector <WiFiManagerParameter*> &wifiParameters,
                         bool resetSettings)
{
  // With parameters, reset settings
  WiFiManager wifiManager;
  
  if (resetSettings)
  {
    wifiManager.resetSettings();
  }

  bool loadedFromFS = parameterSetup(wifiManager, wifiParameters);
  // Force captive protal if parms not loaded from FS
  wmSetup(wifiManager, !loadedFromFS);
  
  if (WifiManager::_shouldSaveConfig)
  {
    Serial.println("Saving Wifi config...");
    saveConfig(wifiParameters);
  }
}

bool WifiManager::parameterSetup(WiFiManager &wifiManager, std::vector <WiFiManagerParameter*> &wifiParameters)
{
  // Attempt to load defaults from fs
  bool loadedFromFS = loadFromFS(wifiParameters);
  
  Serial.println("Loading WifiManagerParameters...");
  for (std::vector<WiFiManagerParameter*>::size_type a = 0; 
       a != wifiParameters.size(); 
       a++)
  {
    WiFiManagerParameter *parm = new WiFiManagerParameter(
      wifiParameters[a]->getID(),
      wifiParameters[a]->getPlaceholder(),
      wifiParameters[a]->getValue(),
      wifiParameters[a]->getValueLength(),
      wifiParameters[a]->getCustomHTML());
    wifiManager.addParameter(parm);
  }
  Serial.println("Done loading WifiManagerParameters...");

  return loadedFromFS;
}

void WifiManager::wmSetup(WiFiManager &wifiManager, bool forceCaptivePortal)
{
  // set callback that gets called when connecting to previous 
  //   WiFi fails, and enters Access Point mode
  wifiManager.setAPCallback(WifiManager::configModeCallback);
  wifiManager.setSaveConfigCallback(WifiManager::saveConfigCallback);  
  
  if ( forceCaptivePortal )
  {
    Serial.println("Starting WifiManager captive portal...");
    if (!wifiManager.startConfigPortal())
    {
      Serial.println("Failed to connect and hit timeout");
      ESP.reset();
      delay(1000);
    }
  }
  else
  {
    // fetches ssid and pass and tries to connect
    //   if it does not connect it starts an access point with the specified name
    //   and goes into a blocking loop awaiting configuration
    Serial.println("Starting WifiManager autoConnect()...");
    if (!wifiManager.autoConnect())
    {
      Serial.println("Failed to connect and hit timeout");
      ESP.reset();
      delay(1000);
    }
    Serial.println("Finished WifiManager autoConnect()...");
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
  Serial.println("Setting _shouldSaveConfig = true");
  WifiManager::_shouldSaveConfig = true;
}

void WifiManager::saveConfig(std::vector <WiFiManagerParameter*> &wifiParameters)
{
  Filesystem fs;
  
  Serial.println("Saving config to fs");
  
  fs.printAllKeys();
  for (unsigned int a = 0;
       a != wifiParameters.size(); 
       ++a)
  {
    fs.updateKey(wifiParameters[a]->getID(), 
                 wifiParameters[a]->getValue());
  }
  
  fs.flushToFs();
  fs.printAllKeys();
}

bool WifiManager::loadFromFS(std::vector <WiFiManagerParameter*> &wifiParameters)
{
  bool retVal = true;
  Filesystem fs;
  for (unsigned int i=0; i < wifiParameters.size(); ++i)
  {
    Serial.println(wifiParameters[i]->getID());
    std::string savedValue = fs.loadFromFs(wifiParameters[i]->getID()); 
    if (savedValue != "")
    {
      Serial.println(wifiParameters[i]->getID());
      WiFiManagerParameter *newParam = new 
        WiFiManagerParameter(wifiParameters[i]->getID(),
                             savedValue.c_str(),
                             savedValue.c_str(),
                             savedValue.size()+1);
      wifiParameters[i] = newParam;
    }
    else
    {
      Serial.println(wifiParameters[i]->getID());
      retVal = false;
    }
  }

  return retVal;
}
