#include "gtest/gtest.h"

#include "MetricTemp.h"

namespace {
  TEST(MetricTemp, EmptyJson) {
    MetricTemp mt;
    EXPECT_STREQ(NULL, mt.getJSON().c_str());
    
  }
}