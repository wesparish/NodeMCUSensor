/**
 * Project NodeMCUSensor
 */


#ifndef _SENSORBASE_H
#define _SENSORBASE_H

#include "MetricBase.h"

class SensorBase {
public: 
  
  bool readSensor(MetricBase &metricBase);
};

#endif //_SENSORBASE_H
