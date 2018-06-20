/**
 * Project NodeMCUSensor
 */


#ifndef _SENSORBASE_H
#define _SENSORBASE_H

#include "MetricBase.h"

class SensorBase {
public: 
  virtual bool readSensor(MetricBase &metricBase) = 0;
};

#endif //_SENSORBASE_H
