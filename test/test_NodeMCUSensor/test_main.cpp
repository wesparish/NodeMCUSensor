#include <Arduino.h>
#include <unity.h>

#include "MetricTemp.h"
#include "Elasticsearch.h"
#include "WifiManager.h"

#include <ESP8266WiFi.h>

// Test libraries
#include <ctime>
#include <sstream>

using namespace std;

#include "secret.h"

Elasticsearch *es;
WifiManager *wm;

//////////////////
// WIFI SUPPORT //
//////////////////
void onSTAConnected (WiFiEventStationModeConnected ipInfo) {
    Serial.printf ("Connected to %s\r\n", ipInfo.ssid.c_str ());
}

// Start NTP only after IP network is connected
void onSTAGotIP (WiFiEventStationModeGotIP ipInfo) {
    Serial.printf ("Got IP: %s\r\n", ipInfo.ip.toString ().c_str ());
    Serial.printf ("Connected: %s\r\n", WiFi.status () == WL_CONNECTED ? "yes" : "no");
}

// Manage network disconnection
void onSTADisconnected (WiFiEventStationModeDisconnected event_info) {
    Serial.printf ("Disconnected from SSID: %s\n", event_info.ssid.c_str ());
    Serial.printf ("Reason: %d\n", event_info.reason);
}
//////////////////
// WIFI SUPPORT //
//////////////////

void setUp(void) {
  static WiFiEventHandler e1, e2, e3;

  WiFi.mode (WIFI_STA);
  WiFi.begin (YOUR_WIFI_SSID, YOUR_WIFI_PASSWD);

  e1 = WiFi.onStationModeGotIP (onSTAGotIP);
  e2 = WiFi.onStationModeDisconnected (onSTADisconnected);
  e3 = WiFi.onStationModeConnected (onSTAConnected);

  Serial.print("WiFi connecting...");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

//  wm = new WifiManager();
//  es = new Elasticsearch("TEST",
//                         "elasticsearch.weshouse:9200",
//                         "unittest");

  NTP.begin();
  Serial.println("DONE wes");
}

// void tearDown(void) {
// // clean stuff up here
// }

void test_MetricTemp_getJson(void) {
  // Load time from NTP
  int hour=0, minute=0, second=0, month=0, day=0, year=0;
  sscanf(NTP.getTimeDateString().c_str(),
         "%02d:%02d:%02d %02d/%02d/%04d",
         &hour, &minute, &second,
         &day, &month, &year);
  // Build time string from NTP variables
  char currentTime[32] = {0};
  sprintf(currentTime,
          "%04d-%02d-%02dT%02d:%02d:%02dZ",
          year, month, day,
          hour, minute, second);

  MetricTemp mt;
  string expectedJson = "{\"location\":\"\","
                              "\"temp\":0,"
                              "\"humidity\":0,"
                              "\"heat_index\":0,"
                              "\"@timestamp\":\"" + 
                              std::string(currentTime) + 
                              "\"}";
  TEST_ASSERT_EQUAL_STRING(mt.getJSON().c_str(), expectedJson.c_str());
}


void setup() {
    UNITY_BEGIN();

    RUN_TEST(test_MetricTemp_getJson);

    UNITY_END();
}

void loop() {
}

