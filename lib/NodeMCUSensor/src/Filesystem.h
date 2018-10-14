/**
 * Project NodeMCUSensor
 */

#ifndef _FILESYSTEM_H
#define _FILESYSTEM_H

#include <FS.h>
#include "ArduinoJson.h"

#include <map>

class Filesystem
{
public:
  Filesystem();

  // Returns a value from FS, given a key
  std::string loadFromFs(std::string key);
  
  // Saves a KV pair to FS (append or overwrite)
  bool updateKey(std::string key, std::string value);

  // Removes a KV pair from FS (delete if exists)
  bool deleteKey(std::string key);
  
  // Flushes data out to filesystem
  bool flushToFs();
protected:
  // Read all KVs from SPIFFS
  std::map <std::string, std::string> readFromSPIFFS();
  
private:
  std::map <std::string, std::string> _kvPairs;
};

#endif
