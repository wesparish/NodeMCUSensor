/**
 * Project NodeMCUSensor
 */

#ifndef _METRICTEMP_H
#define _METRICTEMP_H

// System includes
#include <string>

// Library includes
#ifdef UNITTEST
#include <time.h>
#include <stdio.h>
#else
#include "NtpClientLib.h"
#include "TimeLib.h"
#endif

#include "ArduinoJson.h"

// Custom includes
#include "MetricBase.h"

class MetricTemp: public MetricBase {
public:
  std::string getJSON();

protected:

private: 
  float temp;
  float humidity;
  std::string location;
  float heatIndex;
};

#endif //_METRICTEMP_H
