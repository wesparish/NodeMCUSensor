/**
 * Project NodeMCUSensor
 */


#include "Elasticsearch.h"

/**
 * Elasticsearch implementation
 * 
 * Elasticsearch interface class
 */

/**
 * constructor
 */
Elasticsearch::Elasticsearch(std::string indexBasename,
		             std::string elasticsearchURL)
{
  _indexBasename = indexBasename;
  _elasticsearchURL = elasticsearchURL;
}


/**
 * @param metricBase 
 * @return bool
 */
bool Elasticsearch::indexRecord(MetricBase metricBase ) {
  return false;
}

/**
 * @return bool
 */
bool Elasticsearch::startNTP() {
  NTP.begin();

  bool ntpSynced = false;

  NTP.onNTPSyncEvent ([](NTPSyncEvent_t event)
  {
    if (event)
    {
      Serial.print ("Time Sync error: ");
      if (event == noResponse)
        Serial.println ("NTP server not reachable");
      else if (event == invalidAddress)
        Serial.println ("Invalid NTP server address");
    }
    else
    {
      Serial.print ("Got NTP time: ");
      Serial.println (NTP.getTimeDateString (NTP.getLastNTPSync ()));
    }
  });

  while ( !ntpSynced )
  {
    delay(500);
  }

  return true;
}
