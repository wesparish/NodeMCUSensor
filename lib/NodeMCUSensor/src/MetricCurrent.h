/**
 * Project NodeMCUSensor
 */

#ifndef _METRICCURRENT_H
#define _METRICCURRENT_H

// System includes
#include <string>

// Library includes
#include "NtpClientLib.h"
#include "TimeLib.h"
#include "ArduinoJson.h"

// Custom includes
#include "MetricBase.h"

class MetricCurrent: public MetricBase {
public:
  MetricCurrent();
  /*MetricCurrent(float temp,
                float humidity,
                std::string location,
                float heatIndex);*/

  std::string getJSON();
  std::string getType();
private: 
  float current;
  /*float temp;
  float humidity;
  std::string location;
  float heatIndex;*/
};

#endif //_METRICCURRENT_H
