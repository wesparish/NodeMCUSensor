#define ARDUINO 101
#define NETWORK_TYPE NETWORK_ESP8266

#include "gtest/gtest.h"

#include "MetricTemp.h"

namespace {
  TEST(MetricTemp, EmptyJson) {
    MetricTemp mt;
    EXPECT_STREQ(NULL, mt.getJSON().c_str());
    
  }
}