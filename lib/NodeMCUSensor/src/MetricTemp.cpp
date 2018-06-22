/**
 * Project NodeMCUSensor
 */


#include "MetricTemp.h"

/**
 * MetricTemp implementation
 * 
 * Metric returned by a temperature sensor
 */

std::string
MetricTemp::getType()
{
  return "MetricTemp";
}

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

void
MetricTemp::setTemp(float temp)
{
  this->temp = temp;
}

void
MetricTemp::setHumidity(float humidity)
{
  this->humidity = humidity;
}

void
MetricTemp::setHeatIndex(float heatIndex)
{
  this->heatIndex = heatIndex;
}

float
MetricTemp::getTemp()
{
  return this->temp;
}

float
MetricTemp::getHumidity()
{
  return this->humidity;
}

float
MetricTemp::getHeatIndex()
{
  return this->heatIndex;
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
