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
  StaticJsonDocument<128> jsonDocument;

  jsonDocument["current"] = this->current;

  char jsonCharStr[128] = {0};
  serializeJson(jsonDocument, jsonCharStr);
  std::string jsonString(jsonCharStr);

  return jsonString;
}
