/**
 * Project NodeMCUSensor
 */


#ifndef _METRICTEMP_H
#define _METRICTEMP_H

#include <string>

#include "MetricBase.h"

class MetricTemp: public MetricBase {
private: 
  float temp;
  float humidity;
  std::string location;
  float heatIndex;
};

#endif //_METRICTEMP_H
