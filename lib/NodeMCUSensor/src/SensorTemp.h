/**
 * Project NodeMCUSensor
 */


#ifndef _SENSORTEMP_H
#define _SENSORTEMP_H

#include "SensorBase.h"

#include "MetricTemp.h"

//for DHT22
#include <DHT.h>

class SensorTemp: public SensorBase {
public:
  SensorTemp();
  SensorTemp(int dhtPin);
  SensorTemp(int dhtPin, int dhtType);

  virtual ~SensorTemp();

  virtual bool readSensor(MetricBase &metricBase);
private:
  DHT *dht;
  float lastTemp;
  float lastHumidity;
  float lastHeatIndex;
};

#endif //_SENSORTEMP_H
