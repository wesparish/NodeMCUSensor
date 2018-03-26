//#include <iostream>

// Includes
#include "Elasticsearch.h"
#include "SensorTemp.h"
#include "MetricTemp.h"

// Defines
#define SLEEP_TIME 10000

// Globals
Elasticsearch *es = 0;
SensorTemp    *tSensor = 0;

void setup()
{
  
}

void loop()
{
  MetricTemp metricTemp;
  if ( tSensor->readSensor(metricTemp) )
  {
    // Successfully read temp, send it
    es->indexRecord( metricTemp );
  }

  delay(SLEEP_TIME);
}

