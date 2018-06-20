/**
 * Project NodeMCUSensor
 */


#ifndef _SENSORTEMP_H
#define _SENSORTEMP_H

#include "SensorBase.h"


class SensorTemp: public SensorBase {
public:
  virtual bool readSensor(MetricBase &metricBase);
};

#endif //_SENSORTEMP_H
