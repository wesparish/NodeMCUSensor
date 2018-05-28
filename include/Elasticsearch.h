/**
 * Project NodeMCUSensor
 */


#ifndef _ELASTICSEARCH_H
#define _ELASTICSEARCH_H

// System includes
#include <string>
#include <time.h>

// Library includes
#ifndef UNITTEST
#include "NtpClientLib.h"
#include "TimeLib.h"
#include <ESP8266HTTPClient.h>
#endif

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

  virtual ~Elasticsearch(){}

  /**
   * @param metricBase
   */
  virtual bool indexRecord(MetricBase &metricBase) = 0;
  std::string getFullURL();
protected:
private: 
  bool httpPost(std::string payload);

  std::string _indexBasename;
  std::string _elasticsearchURL;
  std::string _location;
  
  bool startNTP();
};

class MockElasticsearch : public Elasticsearch
{
  public:
    MockElasticsearch(std::string indexBasename,
                      std::string elasticsearchURL,
                      std::string location) :
      Elasticsearch(indexBasename, elasticsearchURL, location) {}
  
    MOCK_METHOD1(indexRecord, bool(MetricBase &metricBase));
};

#endif //_ELASTICSEARCH_H
