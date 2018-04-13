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
  // If we fail to mount, format it first
  if (!SPIFFS.begin())
  {
    if (!SPIFFS.format())
    {
      Serial.println("FAILED to format SPIFFS");
    }
  }

  Serial.println("mounting FS...");
  if (SPIFFS.begin()) {
    Serial.println("mounted file system");
    if (SPIFFS.exists("/config.json")) {
      //file exists, reading and loading
      Serial.println("reading config file");
      File configFile = SPIFFS.open("/config.json", "r");
      if (configFile) {
        Serial.println("opened config file");
        size_t size = configFile.size();
        // Allocate a buffer to store contents of the file.
        std::unique_ptr<char[]> buf(new char[size]);

        configFile.readBytes(buf.get(), size);
        configFile.close();
        DynamicJsonBuffer jsonBuffer;
        JsonObject& json = jsonBuffer.parseObject(buf.get());
        json.printTo(Serial);
        if (json.success()) {
          Serial.println("\nparsed json");

          //_jsonConfigFileData = json;
        } else {
          Serial.println("failed to load json config");
        }
      }
    }
  } else {
    Serial.println("failed to mount FS");
  }
}