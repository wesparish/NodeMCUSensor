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
#define SWITCHPIN 4

// Globals
Elasticsearch *es = 0;
SensorTemp    *tSensor = 0;
WifiManager   *wifiManager = 0;

std::string esIndexName = "";
std::string esHost = "";
std::string esLocation = "";

void setup()
{
  Serial.begin(115200);
  Serial.println("[debug] Starting setup()");

  WiFiManagerParameter _esIndexName("esIndexName", "tempsensor", "tempsensor", 32);
  WiFiManagerParameter _esHost("esHost", "http://elasticsearch.jamiehouse:9200", "http://elasticsearch.jamiehouse:9200", 64);
  WiFiManagerParameter _esLocation("esLocation", "barn", "barn", 32);

  std::vector <WiFiManagerParameter*> wifiParms;
  wifiParms.push_back(&_esIndexName);
  wifiParms.push_back(&_esHost);
  wifiParms.push_back(&_esLocation);

  // Reset settings if switch is held down on startup
  bool resetSettings = false;
  pinMode(D2, INPUT);
  if (!digitalRead(SWITCHPIN))
  {
    Serial.println("[debug] Button press detected, resetting settings!!");
    resetSettings = true;
    Filesystem fs;
    fs.deleteConfigFile();
  }
  
  wifiManager = new WifiManager(wifiParms, resetSettings);

  Serial.printf("_esIndexName: %s\n", _esIndexName.getValue());
  Serial.printf("_esHost: %s\n", _esHost.getValue());
  Serial.printf("_esLocation: %s\n", _esLocation.getValue());

  es = new Elasticsearch(_esIndexName.getValue(), 
                         _esHost.getValue(), 
                         _esLocation.getValue());

  tSensor = new SensorTemp();

  esIndexName = _esIndexName.getValue();
  esHost = _esHost.getValue();
  esLocation = _esLocation.getValue();
}

void loop()
{
  Serial.println("[debug] In loop()");

  MetricTemp metricTemp;
  Serial.println("[debug] 1");
  metricTemp.setLocation(esLocation);
  Serial.println("[debug] 2");
  if ( tSensor->readSensor(metricTemp) )
  {
    Serial.print("[debug] Read MetricTemp(): ");
    Serial.println(metricTemp.getJSON().c_str());
    Serial.println("[debug] 3");
    // Successfully read temp, send it
    if ( es->indexRecord( metricTemp ) )
    {
      Serial.println("[debug] 4");
      Serial.println("Successfully indexed record to Elasticsearch!");
    }
    else
    {
      Serial.println("[debug] 5");
      Serial.println("Error: Failed to index record to Elasticsearch!");
    }
  }
  else
  {
    Serial.println("[debug] 6");
    Serial.println("Error reading sensor! Not sending metric.");
  }
  
  Serial.println("[debug] 7");
  delay(SLEEP_TIME);
  Serial.println("[debug] 8");
}
