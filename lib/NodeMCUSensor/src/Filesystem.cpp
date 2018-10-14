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
  _kvPairs = readFromSPIFFS();
}

std::string
Filesystem::loadFromFs(std::string key)
{
  std::string retVal = "";
  if(_kvPairs.find(key) != _kvPairs.end())
  {
    retVal = _kvPairs[key];
  }
  return retVal;
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

bool
Filesystem::updateKey(std::string key, std::string value)
{
  _kvPairs[key] = value;
  return true;
}

bool
Filesystem::deleteKey(std::string key)
{
  _kvPairs.erase(_kvPairs.find(key));
  return true;
}

bool
Filesystem::flushToFs()
{
  DynamicJsonBuffer jsonBuffer;
  JsonObject& json = jsonBuffer.createObject();

  std::map<std::string, std::string>::iterator i;
  for ( i = _kvPairs.begin();
        i != _kvPairs.end();
        i++ )
  {
      json[i->first.c_str()] = i->second.c_str();
  }

  File configFile = SPIFFS.open("/config.json", "w");
  if (!configFile) {
    Serial.println("failed to open config file for writing");
  }

  json.printTo(Serial);
  json.printTo(configFile);
  configFile.close();
  return true;
}
