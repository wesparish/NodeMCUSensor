/**
 * Project NodeMCUSensor
 */


#ifndef _METRICBASE_H
#define _METRICBASE_H

#include <string>

class MetricBase {
public: 
  
  virtual std::string getJSON() = 0;
};

#endif //_METRICBASE_H
