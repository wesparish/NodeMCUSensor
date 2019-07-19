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
  
  virtual std::string getJSON() = 0;
  virtual std::string getTimestamp();
  virtual std::string getType() = 0;

  virtual std::string getLocation();
  virtual void setLocation(std::string location);

protected:
  std::string location = "";
};

#endif //_METRICBASE_H
