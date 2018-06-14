/**
 * Project NodeMCUSensor
 */


#ifndef _METRICBASE_H
#define _METRICBASE_H

#include <string>
#include "NtpClientLib.h"
#include "TimeLib.h"

class MetricBase {
public: 
  
  std::string getJSON();
  std::string getTimestamp();
};

#endif //_METRICBASE_H
