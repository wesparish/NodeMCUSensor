/**
 * Project NodeMCUSensor
 */


#ifndef _ELASTICSEARCH_H
#define _ELASTICSEARCH_H

#include <string>

#include "MetricBase.h"

class Elasticsearch {
public: 
  
  /**
   * @param metricBase
   */
  bool indexRecord(MetricBase metricBase );
private: 
  std::string indexBasename;
  std::string elasticsearchURL;
  
  bool startNTP();
};

#endif //_ELASTICSEARCH_H
