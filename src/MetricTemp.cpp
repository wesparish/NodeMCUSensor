/**
 * Project NodeMCUSensor
 */


#include "MetricTemp.h"

/**
 * MetricTemp implementation
 * 
 * Metric returned by a temperature sensor
 */

std::string MetricTemp::getJSON()
{
  int hour=0, minute=0, second=0, month=0, day=0, year=0;
  sscanf(NTP.getTimeDateString().c_str(),
         "%02d:%02d:%02d %02d/%02d/%04d",
         &hour, &minute, &second,
         &day, &month, &year);

  char timestamp[32] = {0};
  sprintf(timestamp,
          "%04d-%02d-%02dT%02d:%02d:%02dZ",
          year, month, day,
          hour, minute, second);

  DynamicJsonBuffer jsonBuffer;
  JsonObject& json = jsonBuffer.createObject();
  json["location"] = location.c_str();
  json["temp"] = temp;
  json["humidity"] = humidity;
  json["heat_index"] = heatIndex;
  json["@timestamp"] = timestamp;

  std::string jsonString;
  json.printTo(jsonString);

  return jsonString;
}
