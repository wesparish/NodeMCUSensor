/**
 * Project NodeMCUSensor
 */


#include "MetricCurrent.h"

/**
 * MetricCurrent implementation
 * 
 * Metric returned by a current sensor
 */

std::string
MetricCurrent::getType()
{
  return "MetricCurrent";
}

MetricCurrent::MetricCurrent()
{
}
/*
MetricCurrent::MetricCurrent(float temp,
                             float humidity,
                             std::string location,
                             float heatIndex)
{
  this->temp = temp;
  this->humidity = humidity;
  this->location = location;
  this->heatIndex = heatIndex;
}
*/
std::string MetricCurrent::getJSON()
{
  DynamicJsonBuffer jsonBuffer;
  JsonObject& json = jsonBuffer.createObject();
  /*json["location"] = location.c_str();
  json["temp"] = temp;
  json["humidity"] = humidity;
  json["heat_index"] = heatIndex;
  json["@timestamp"] = getTimestamp().c_str();*/

  char tmpStr[128] = {0};
  json.printTo(tmpStr);
  std::string jsonString(tmpStr);

  return jsonString;
}
