/**
 * Project NodeMCUSensor
 */


#include "MetricTemp.h"

/**
 * MetricTemp implementation
 * 
 * Metric returned by a temperature sensor
 */

MetricTemp::MetricTemp()
{
  this->temp = 0;
  this->humidity = 0;
  this->location = "";
  this->heatIndex = 0.0;
}

MetricTemp::MetricTemp(float temp,
                       float humidity,
                       std::string location,
                       float heatIndex)
{
  this->temp = temp;
  this->humidity = humidity;
  this->location = location;
  this->heatIndex = heatIndex;
}

std::string MetricTemp::getJSON()
{
  DynamicJsonBuffer jsonBuffer;
  JsonObject& json = jsonBuffer.createObject();
  json["location"] = location.c_str();
  json["temp"] = temp;
  json["humidity"] = humidity;
  json["heat_index"] = heatIndex;
  json["@timestamp"] = getTimestamp().c_str();

  char tmpStr[128] = {0};
  json.printTo(tmpStr);
  std::string jsonString(tmpStr);

  return jsonString;
}
