/**
 * Project NodeMCUSensor
 */

#ifndef _METRICTEMP_H
#define _METRICTEMP_H

// System includes
#include <string>

// Library includes
#include "TimeLib.h"
#include "ArduinoJson.h"

// Custom includes
#include "MetricBase.h"

class MetricTemp: public MetricBase {
public:
  MetricTemp();
  MetricTemp(float temp,
             float humidity,
             std::string location,
             float heatIndex);

  std::string getJSON();
  std::string getType();

  void setTemp(float temp);
  void setHumidity(float humidity);
  void setHeatIndex(float heatIndex);

  float getTemp();
  float getHumidity();
  float getHeatIndex();
private: 
  float temp;
  float humidity;
  std::string location;
  float heatIndex;
};

#endif //_METRICTEMP_H
