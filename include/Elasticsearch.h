/**
 * Project NodeMCUSensor
 */


#ifndef _ELASTICSEARCH_H
#define _ELASTICSEARCH_H

// System includes
#include <string>

// Library includes
#include "NtpClientLib.h"

// Custom includes
#include "MetricBase.h"

class Elasticsearch {
public:

  /**
   * constructor
   */
  Elasticsearch(std::string indexBasename,
		std::string elasticsearchURL);

  /**
   * @param metricBase
   */
  bool indexRecord(MetricBase metricBase );
private: 
  std::string _indexBasename;
  std::string _elasticsearchURL;
  
  bool startNTP();
};

#endif //_ELASTICSEARCH_H
