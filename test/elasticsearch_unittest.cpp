
// gmock
#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "Elasticsearch.h"

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