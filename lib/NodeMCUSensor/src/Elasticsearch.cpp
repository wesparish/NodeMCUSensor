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
		                     std::string elasticsearchURL,
                             std::string location)
{
  _indexBasename = indexBasename;
  _elasticsearchURL = elasticsearchURL;
  _location = location;
  
  startNTP();
}


/**
 * @param metricBase 
 * @return bool
 */
bool Elasticsearch::indexRecord(MetricBase &metricToSend)
{
  std::string payload = metricToSend.getJSON();
  return httpPost(payload);
}

bool Elasticsearch::httpPost(std::string payload)
{
  int hour=0, minute=0, second=0, month=0, day=0, year=0;
  sscanf(NTP.getTimeDateString().c_str(),
         "%02d:%02d:%02d %02d/%02d/%04d",
         &hour, &minute, &second,
         &day, &month, &year);

  char fullUrl[128] = {0};
  sprintf(fullUrl,
          "http://%s/%s-%s-%04d.%02d.%02d/doc",
          _elasticsearchURL.c_str(),
          _indexBasename.c_str(),
          _location.c_str(),
          year, month, day);

  Serial.println(fullUrl);

  int httpCode = -1;
  HTTPClient http;
  http.begin(fullUrl);
  http.addHeader("Content-Type", String("application/json"));
  httpCode = http.POST(payload.c_str());
  http.end();

  Serial.printf("[%d] %s\n", httpCode, http.getString().c_str());
}


/**
 * @return bool
 */
bool Elasticsearch::startNTP()
{
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
