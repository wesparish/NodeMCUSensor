/**
 * Project NodeMCUSensor
 */


#include "MetricTemp.h"

MetricTemp::MetricTemp()
{
  this->temp = 0;
  this->humidity = 0;
  this->location = "";
  this->heatIndex = 0.0;
}

MetricTemp::MetricTemp(int temp,
                       int humidity,
                       std::string location,
                       float heatIndex)
{
  this->temp = temp;
  this->humidity = humidity;
  this->location = location;
  this->heatIndex = heatIndex;
}

/**
 * MetricTemp implementation
 * 
 * Metric returned by a temperature sensor
 */

std::string MetricTemp::getJSON()
{
  int hour=0, minute=0, second=0, month=0, day=0, year=0;
  
  #ifdef UNITTEST
  // LOAD TIME FOR UNIT TESTING
  time_t rawtime = 0;
  struct tm *timeinfo;
  timeinfo = localtime(&rawtime);
  hour = timeinfo->tm_hour;
  minute = timeinfo->tm_min;
  second = timeinfo->tm_sec;
  month = timeinfo->tm_mon;
  day = timeinfo->tm_yday;
  year = timeinfo->tm_year + 1900;
  #else
  // USE ARDUINO LIBRARY
  sscanf(NTP.getTimeDateString().c_str(),
         "%02d:%02d:%02d %02d/%02d/%04d",
         &hour, &minute, &second,
         &day, &month, &year);
  #endif  

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

  char tmpStr[128] = {0};
  json.printTo(tmpStr);
  std::string jsonString(tmpStr);

  return jsonString;
}
