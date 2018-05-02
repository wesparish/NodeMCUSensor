//#include <iostream>

// Includes
#include "Elasticsearch.h"
#include "SensorTemp.h"
#include "MetricTemp.h"
#include "WifiManager.h"

// Defines
#define SLEEP_TIME 10000

// Globals
Elasticsearch *es = 0;
SensorTemp    *tSensor = 0;
WifiManager   *wifiManager = 0;

void setup()
{
  es = new Elasticsearch("wessvrroom", 
                         "elasticsearch.weshouse:9200", 
                         "weshouse");
  tSensor = new SensorTemp();
  wifiManager = new WifiManager();
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

