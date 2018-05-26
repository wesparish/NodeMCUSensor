
// gmock
#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "Elasticsearch.h"

//using ::testing::Foo;
//using namespace testing;
using ::testing::AtLeast;

class MockElasticsearch : public Elasticsearch
{
  public:
    MockElasticsearch(std::string indexBasename,
                      std::string elasticsearchURL,
                      std::string location) :
      Elasticsearch(indexBasename, elasticsearchURL, location) {}
  
    MOCK_METHOD1(indexRecord, bool(MetricBase &metricBase));
};

namespace {
  TEST(Elasticsearch, MockConstructor) {
    MockElasticsearch es("testindex",
		                     "http://eshost:1234",
                         "testlocation");
    ASSERT_TRUE(true);
  }
  
  TEST(Elasticsearch, getFullURL) {
    MockElasticsearch es("testindex",
		                     "eshost:1234",
                         "testlocation");
    std::string expectedStr = "http://eshost:1234/testindex-testlocation-1970.00.00/doc";
    EXPECT_STREQ(expectedStr.c_str(), es.getFullURL().c_str());
  }

}