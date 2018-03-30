/**
 * Project NodeMCUSensor
 */


#ifndef _METRICTEMP_H
#define _METRICTEMP_H

// System includes
#include <string>

// Library includes
#include "NtpClientLib.h"
#include "TimeLib.h"
#include <ArduinoJson.h>

// Custom includes
#include "MetricBase.h"

class MetricTemp: public MetricBase {
public:
  std::string getJSON();

private: 
  float temp;
  float humidity;
  std::string location;
  float heatIndex;
};

#endif //_METRICTEMP_H
