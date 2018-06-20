/**
 * Project NodeMCUSensor
 */


#include "SensorTemp.h"

/**
 * SensorTemp implementation
 * 
 * Temperature sensor
 */

bool 
SensorTemp::readSensor(MetricBase &metricBase)
{
  if (metricBase.getType() == "MetricTemp")
  {
    Serial.printf("\nSensorTemp::readSensor() received a MetricTemp type!\n");
    return true;
  }
  else
  {
    Serial.printf("\nERROR - SensorTemp::readSensor() received a NON MetricTemp type!\n");
    return false;
  }
}
