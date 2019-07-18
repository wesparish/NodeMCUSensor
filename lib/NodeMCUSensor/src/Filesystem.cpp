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
  printAllKeys();
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

  Serial.println("Mounting FS...");
  if (SPIFFS.begin()) {
    Serial.println("Mounted file system");
    if (SPIFFS.exists("/config.json"))
    {
      //file exists, reading and loading
      Serial.println("Reading config file");
      File configFile = SPIFFS.open("/config.json", "r");
      if (configFile)
      {
        Serial.println("Opened config file");
        size_t size = configFile.size();
        // Allocate a buffer to store contents of the file.
        std::unique_ptr<char[]> buf(new char[size]);

        configFile.readBytes(buf.get(), size);
        configFile.close();
        StaticJsonDocument<512> jsonDocument;
        DeserializationError err = deserializeJson(jsonDocument, buf.get());
        if (!err) {
          serializeJson(jsonDocument, Serial);
          Serial.println();
          Serial.println("Parsed json");

          JsonObject obj = jsonDocument.as<JsonObject>();
          for (JsonObject::iterator it=obj.begin(); it!=obj.end(); ++it) {
            retVal[it->key().c_str()] = it->value().as<char*>();
          }
          printAllKeys();
        } else
        {
          Serial.print("Failed to load json config: ");
          Serial.println(err.c_str());
        }
      }
    }
  }
  else
  {
    Serial.println("Failed to mount FS");
  }

  return retVal;
}

bool
Filesystem::updateKey(std::string key, std::string value)
{
  _kvPairs[key] = value;
  return flushToFs();
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
  bool retVal = true;
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
    Serial.println("Failed to open config file for writing");
    retVal = false;
  }

  serializeJson(jsonDocument, Serial);
  serializeJson(jsonDocument, configFile);
  configFile.close();
  return retVal;
}
