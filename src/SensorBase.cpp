/**
 * Project NodeMCUSensor
 */


#include "SensorBase.h"

/**
 * SensorBase implementation
 * 
 * Base class for a sensor that provides some kind of on-demand reading
 */


/**
 * @return MetricBase
 */
bool SensorBase::readSensor(MetricBase &metricBase) {
  metricBase = MetricBase();
  return true;
}
