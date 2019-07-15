// Included for PlatformIO
//#include <SPI.h>

// Includes
#include "Elasticsearch.h"
#include "SensorTemp.h"
#include "MetricTemp.h"
#include "WifiManager.h"
#include "Filesystem.h"

// Defines
#define SLEEP_TIME 10000

// Globals
Elasticsearch *es = 0;
SensorTemp    *tSensor = 0;
WifiManager   *wifiManager = 0;

void setup()
{
  Serial.begin(115200);
  Serial.println("[debug] Starting setup()");

  Filesystem filesystem;
  std::string indexName(filesystem.loadFromFs("indexName"));
  std::string esHost(filesystem.loadFromFs("esHost"));
  std::string location(filesystem.loadFromFs("location"));

  if (! indexName.compare("") ) {
    Serial.println("Could not load indexName from fs");
  }
  if (! esHost.compare("") ) {
    Serial.println("Could not load esHost from fs");
  }
  if (! location.compare("") ) {
    Serial.println("Could not load location from fs");
  }

  es = new Elasticsearch("wessvrroom", 
                         "elasticsearch.weshouse:9200", 
                         "weshouse");
  tSensor = new SensorTemp();
  wifiManager = new WifiManager();
}

void loop()
{
  Serial.println("[debug] In loop()");

  MetricTemp metricTemp;
  if ( tSensor->readSensor(metricTemp) )
  {
    Serial.print("[debug] Read MetricTemp(): ");
    Serial.println(metricTemp.getJSON().c_str());
    // Successfully read temp, send it
    es->indexRecord( metricTemp );
  }

  delay(SLEEP_TIME);
}
