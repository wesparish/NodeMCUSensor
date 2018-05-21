#include "gtest/gtest.h"

#include "MetricTemp.h"

namespace {
  TEST(MetricTemp, DefaultConstructor) {
    MetricTemp mt;
    std::string expectedStr = "{\"location\":\"\","
                               "\"temp\":0,"
                               "\"humidity\":0,"
                               "\"heat_index\":0,"
                               "\"@timestamp\":\"1970-00-00T00:00:00Z\"}";
    EXPECT_STREQ(expectedStr.c_str(), mt.getJSON().c_str());
  }
  
  TEST(MetricTemp, ParmConstructor) {
    MetricTemp mt(99, 89, "TESTLOCATION", 79);
    std::string expectedStr = "{\"location\":\"TESTLOCATION\","
                               "\"temp\":99,"
                               "\"humidity\":89,"
                               "\"heat_index\":79,"
                               "\"@timestamp\":\"1970-00-00T00:00:00Z\"}";
    EXPECT_STREQ(expectedStr.c_str(), mt.getJSON().c_str());
  }
}