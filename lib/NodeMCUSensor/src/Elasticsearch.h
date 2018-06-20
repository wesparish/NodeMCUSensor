/**
 * Project NodeMCUSensor
 */


#ifndef _ELASTICSEARCH_H
#define _ELASTICSEARCH_H

// System includes
#include <string>
#include <time.h>

// Library includes
#include "NtpClientLib.h"
#include "TimeLib.h"
#include <ESP8266HTTPClient.h>

// Custom includes
#include "MetricBase.h"

class Elasticsearch {
public:

  /**
   * constructor
   */
  Elasticsearch(std::string indexBasename,
                std::string elasticsearchURL,
                std::string location);

  /**
   * @param metricBase
   */
  bool indexRecord(MetricBase *metricBase);
  bool indexRecord(MetricBase &metricBase);
  bool indexRecord(std::string payload);

  std::string getFullURL();

private: 
  bool httpPost(std::string const &payload);

  std::string _indexBasename;
  std::string _elasticsearchURL;
  std::string _location;
};

#endif //_ELASTICSEARCH_H
