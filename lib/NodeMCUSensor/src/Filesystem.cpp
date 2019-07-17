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

void
Filesystem::printAllKeys()
{
  Serial.println("Dumping all fs keys");
  std::map<std::string, std::string>::iterator i;
  for ( i = _kvPairs.begin();
        i != _kvPairs.end();
        i++ )
  {
    Serial.print(i->first.c_str());
    Serial.print(" -> ");
    Serial.println(i->second.c_str());
  }
}

std::string
Filesystem::loadFromFs(std::string key)
{
  Serial.print("Looking for key in fs: ");
  Serial.println(key.c_str());
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
        StaticJsonDocument<512> jsonDocument;
        if (DeserializationError::Ok != 
            deserializeJson(jsonDocument, buf.get()) ) {
          serializeJson(jsonDocument, Serial);
          Serial.println("\nparsed json");

          JsonObject obj = jsonDocument.as<JsonObject>();
          for (JsonObject::iterator it=obj.begin(); it!=obj.end(); ++it) {
            retVal[it->key().c_str()] = it->value().as<char*>();
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
  StaticJsonDocument<512> jsonDocument;

  std::map<std::string, std::string>::iterator i;
  for ( i = _kvPairs.begin();
        i != _kvPairs.end();
        i++ )
  {
    jsonDocument[i->first.c_str()] = i->second.c_str();
  }

  File configFile = SPIFFS.open("/config.json", "w");
  if (!configFile) {
    Serial.println("failed to open config file for writing");
  }

  serializeJson(jsonDocument, Serial);
  serializeJson(jsonDocument, configFile);
  configFile.close();
  return true;
}
