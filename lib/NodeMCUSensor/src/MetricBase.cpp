/**
 * Project NodeMCUSensor
 */


#include "MetricBase.h"

/**
 * MetricBase implementation
 * 
 * Base class for any metrics returned from a sensor reading
 */


/**
 * @return string
 */
std::string MetricBase::getJSON() {
  return "";
}

std::string MetricBase::getTimestamp()
{
  int hour=0, minute=0, second=0, month=0, day=0, year=0;
  sscanf(NTP.getTimeDateString().c_str(),
         "%02d:%02d:%02d %02d/%02d/%04d",
         &hour, &minute, &second,
         &day, &month, &year);

  char timestamp[32] = {0};
  sprintf(timestamp,
          "%04d-%02d-%02dT%02d:%02d:%02dZ",
          year, month, day,
          hour, minute, second);

  return std::string(timestamp);
}
