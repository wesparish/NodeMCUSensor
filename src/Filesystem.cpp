/**
 * Project NodeMCUSensor
 */


#include "Filesystem.h"

/**
 * Filesystem implementation
 * 
 * Filesystem management class
 */

/**
 * Constructor
 */
Filesystem::Filesystem()
{
}

std::string 
Filesystem::loadFromFs(std::string key)
{
}

std::map <std::string, std::string> 
Filesystem::readFromSPIFFS()
{
  std::map <std::string, std::string> retVal;
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
    if (SPIFFS.exists("/config.json"))
    {
      //file exists, reading and loading
      Serial.println("reading config file");
      File configFile = SPIFFS.open("/config.json", "r");
      if (configFile) 
      {
        Serial.println("opened config file");
        size_t size = configFile.size();
        // Allocate a buffer to store contents of the file.
        std::unique_ptr<char[]> buf(new char[size]);

        configFile.readBytes(buf.get(), size);
        configFile.close();
        DynamicJsonBuffer jsonBuffer;
        JsonObject& json = jsonBuffer.parseObject(buf.get());
        if (json.success()) {
          json.printTo(Serial);
          Serial.println("\nparsed json");

          //_jsonConfigFileData = json;
          for (JsonObject::iterator it=json.begin(); it!=json.end(); ++it) 
          {
            // Store the KV pair into the return map
            retVal[it->key] = it->value.as<char*>();
          }
        } else 
        {
          Serial.println("failed to load json config");
        }
      }
    }
  } 
  else 
  {
    Serial.println("failed to mount FS");
  }
  
  return retVal;
}