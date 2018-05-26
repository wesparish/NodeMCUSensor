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
  
  #ifndef UNITTEST
  startNTP();
  #endif
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

std::string Elasticsearch::getFullURL()
{
  int hour=0, minute=0, second=0, month=0, day=0, year=0;
  #ifdef UNITTEST
  // LOAD TIME FOR UNIT TESTING
  time_t rawtime = 0;
  struct tm *timeinfo;
  timeinfo = localtime(&rawtime);
  hour = timeinfo->tm_hour;
  minute = timeinfo->tm_min;
  second = timeinfo->tm_sec;
  month = timeinfo->tm_mon;
  day = timeinfo->tm_yday;
  year = timeinfo->tm_year + 1900;
  #else
  sscanf(NTP.getTimeDateString().c_str(),
         "%02d:%02d:%02d %02d/%02d/%04d",
         &hour, &minute, &second,
         &day, &month, &year);
  #endif

  char fullUrl[128] = {0};
  sprintf(fullUrl,
          "http://%s/%s-%s-%04d.%02d.%02d/doc",
          _elasticsearchURL.c_str(),
          _indexBasename.c_str(),
          _location.c_str(),
          year, month, day);
  
  return std::string(fullUrl);
}

bool Elasticsearch::httpPost(std::string payload)
{
  std::string fullUrl = getFullURL();
  #ifndef UNITTEST
  Serial.println(fullUrl);
  
  int httpCode = -1;
  HTTPClient http;
  http.begin(fullUrl);
  http.addHeader("Content-Type", String("application/json"));
  httpCode = http.POST(payload.c_str());
  http.end();

  Serial.printf("[%d] %s\n", httpCode, http.getString().c_str());
  return (httpCode > 200 && httpCode < 400);
  #else
  // UNITTEST
  return payload != "";
  #endif
}


/**
 * @return bool
 */
bool Elasticsearch::startNTP()
{
  #ifndef UNITTEST
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
  #endif

  return true;
}
