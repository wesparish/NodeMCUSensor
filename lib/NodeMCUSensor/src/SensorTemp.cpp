/**
 * Project NodeMCUSensor
 */


#include "SensorTemp.h"

/**
 * SensorTemp implementation
 * 
 * Temperature sensor
 */
SensorTemp::SensorTemp()
{
  dht = new DHT(5, DHT22);
  dht->begin();
}

SensorTemp::SensorTemp(int dhtPin)
{
  dht = new DHT(dhtPin, DHT22);
  dht->begin();
}

SensorTemp::SensorTemp(int dhtPin, int dhtType)
{
  dht = new DHT(dhtPin, dhtType);
  dht->begin();
}

SensorTemp::~SensorTemp()
{
  delete dht;
}

bool 
SensorTemp::readSensor(MetricBase &metricBase)
{
  if (metricBase.getType() == "MetricTemp")
  {
    float f = 0.00;
    float h = 0.00;
    int tryCount = 0;
    do
    {
      if (isnan(h) || isnan(f))
        delay(2000);
      f = dht->readTemperature(true);
      h = dht->readHumidity();

      #ifdef DHT_DEBUG
      Serial.printf("\nf = %f, h = %f\n", f, h);
      #endif
    } while ((isnan(h) || isnan(f)) && tryCount < 5);

    if (isnan(h) || isnan(f))
    {
      Serial.println("Failed to read from DHT sensor!");
      return false;
    }
    else
    {
      // Compute heat index
      float hif = dht->computeHeatIndex(f, h);
      MetricTemp &mt = static_cast<MetricTemp&>(metricBase);
      mt.setTemp(f);
      mt.setHumidity(h);
      mt.setHeatIndex(hif);
      return true;
    }
  }
  else
  {
    Serial.printf("\nERROR - SensorTemp::readSensor() received a NON MetricTemp type!\n");
    return false;
  }
}
