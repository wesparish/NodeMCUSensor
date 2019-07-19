#include <Arduino.h>
#include <unity.h>

#include "MetricTemp.h"
#include "MetricCurrent.h"
#include "Elasticsearch.h"
#include "WifiManager.h"
#include "SensorTemp.h"
#include "Filesystem.h"

#include <ESP8266WiFi.h>

// Test libraries
#include <ctime>
#include <sstream>

using namespace std;

#include "secret.h"

//Elasticsearch *es;
//WifiManager *wm;

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
//   static WiFiEventHandler e1, e2, e3;
// 
//   WiFi.mode (WIFI_STA);
//   WiFi.begin (YOUR_WIFI_SSID, YOUR_WIFI_PASSWD);
// 
//   e1 = WiFi.onStationModeGotIP (onSTAGotIP);
//   e2 = WiFi.onStationModeDisconnected (onSTADisconnected);
//   e3 = WiFi.onStationModeConnected (onSTAConnected);
// 
//   Serial.print("WiFi connecting...");
//   while (WiFi.status() != WL_CONNECTED)
//   {
//     delay(500);
//     Serial.print(".");
//   }
// 
// //  wm = new WifiManager();
// //  es = new Elasticsearch("TEST",
// //                         "elasticsearch.weshouse:9200",
// //                         "unittest");
// 
//   NTP.begin();
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
  MetricTemp mt2(79.1, 40.5, "SunnyLocation", 75.123);
  string expectedJson2 = "{\"location\":\"SunnyLocation\","
                            "\"temp\":79.1,"
                            "\"humidity\":40.5,"
                            "\"heat_index\":75.123,"
                            "\"@timestamp\":\"" + 
                            std::string(currentTime) + 
                            "\"}";
  TEST_ASSERT_EQUAL_STRING(mt.getJSON().c_str(), expectedJson.c_str());
  TEST_ASSERT_EQUAL_STRING(mt2.getJSON().c_str(), expectedJson2.c_str());
}

void test_Elasticsearch_getFullURL(void) {
  // Load time from NTP 
  int hour=0, minute=0, second=0, month=0, day=0, year=0;
  sscanf(NTP.getTimeDateString().c_str(),
         "%02d:%02d:%02d %02d/%02d/%04d",
         &hour, &minute, &second,
         &day, &month, &year);

  Elasticsearch es("testindex",
                   "http://elasticsearch.weshouse:9200",
                   "unittest-location");
  char expectedFullUrl[128] = {0};
  sprintf(expectedFullUrl, "%s/%s-%s-%04d.%02d.%02d/doc",
                           "http://elasticsearch.weshouse:9200",
                           "testindex",
                           "unittest-location",
                           year, month, day);

  TEST_ASSERT_EQUAL_STRING(es.getFullURL().c_str(), 
                           expectedFullUrl);
}

void test_Elasticsearch_indexRecord(void) {
  MetricTemp mt2(79.1, 40.5, "SunnyLocation", 75.123);
  Elasticsearch es("testindex",
                   "http://elasticsearch.weshouse:9200",
                   "unittest");
  TEST_ASSERT_TRUE(es.indexRecord(mt2.getJSON()));
  TEST_ASSERT_TRUE(es.indexRecord(mt2));
  TEST_ASSERT_TRUE(es.indexRecord(&mt2));
}

void test_SensorTemp_readSensor(void) {
  MetricTemp mt;
  MetricCurrent mc;
  SensorTemp st(5,DHT22);
  SensorTemp st2(5);
  SensorTemp st3;

  // Test all constructors
  TEST_ASSERT_TRUE(st.readSensor(mt));
  TEST_ASSERT_TRUE(st2.readSensor(mt));
  TEST_ASSERT_TRUE(st3.readSensor(mt));
  // Should fail to read with the wrong metric type
  TEST_ASSERT_FALSE(st.readSensor(mc));
  // Expect a reasonable temperature, humidity and heat index back
  TEST_ASSERT_TRUE(mt.getTemp() > 50 && mt.getTemp() < 120);
  TEST_ASSERT_TRUE(mt.getHumidity() > 10 && mt.getHumidity() < 100);
  TEST_ASSERT_TRUE(mt.getHeatIndex() > 10 && mt.getHeatIndex() < 120);
}

void test_Filesystem_testKV(void) {
  Filesystem fs;
  // Missing string expected to be ""
  TEST_ASSERT_EQUAL_STRING(fs.loadFromFs("testKey-should_not_exist").c_str(), "");
  // Write a kv pair
  TEST_ASSERT_TRUE(fs.updateKey("testkey", "testvalue"));
  TEST_ASSERT_TRUE(fs.updateKey("testkey2", "testvalue2"));
  // Flush to file
  TEST_ASSERT_TRUE(fs.flushToFs());
  // Read from file
  TEST_ASSERT_EQUAL_STRING(fs.loadFromFs("testkey").c_str(), "testvalue");
  TEST_ASSERT_EQUAL_STRING(fs.loadFromFs("testkey2").c_str(), "testvalue2");
  // Delete test keys
  TEST_ASSERT_TRUE(fs.deleteKey("testkey"));
  TEST_ASSERT_TRUE(fs.deleteKey("testkey2"));
  // Flush to file
  TEST_ASSERT_TRUE(fs.flushToFs());
}

void test_WifiManager_parameters(void) {
  // Test parameter
  {
  WiFiManagerParameter parm1("parm1id", "parm1placeholder", "parm1default", 32);
  }

  WiFiManagerParameter *parm2 = new WiFiManagerParameter("parm1id", "parm1placeholder", "parm1default", 32);
  delete parm2;

  WiFiManagerParameter parm3("parm3id", "parm3placeholder", "parm3default", 32);

  std::vector <WiFiManagerParameter*> parmArray;
  parmArray.push_back(&parm3);
}

void test_WifiManager_testCtor(void) {
  // Test no parameters, default ctor
  WifiManager wm();

  // Test parameters, without reset
  std::vector <WiFiManagerParameter*> wifiParms;
  // WiFiManagerParameter(const char *id, const char *placeholder, const char *defaultValue, int length);
  //   WiFiManagerParameter custom_mqtt_server("server", "mqtt server", mqtt_server, 40);
  char parm1Default[32] = "parm1default";
  char parm2Default[32] = "parm2default";
  WiFiManagerParameter parm1("parm1id", "parm1placeholder", parm1Default, 32, "customHTML1"); 
  WiFiManagerParameter parm2("parm2id", "parm2placeholder", parm2Default, 32, "customHTML2"); 
  wifiParms.push_back(&parm1);
  wifiParms.push_back(&parm2);

  WifiManager wm2(wifiParms);

  // Test parameters, with reset - REQUIRES MANUAL INTERVENTION
  // WifiManager wm3(wifiParms, true);

  // Verify parameters from fs
  WiFiManagerParameter parm3("parm1id", "parm1-testdata", "parm1-testdata", 32);
  WiFiManagerParameter parm4("parm2id", "parm2-testdata", "parm2-testdata", 32);
  std::vector <WiFiManagerParameter*> wifiParms2;
  wifiParms2.push_back(&parm3);
  wifiParms2.push_back(&parm4);

  // Write some test data to SPIFFS to pick up
  wm2.saveConfig(wifiParms2);

  wm2.loadFromFS(wifiParms);

  TEST_ASSERT_EQUAL_STRING("parm1id", wifiParms2[0]->getID());
  TEST_ASSERT_EQUAL_STRING("parm1-testdata", wifiParms2[0]->getPlaceholder());
  TEST_ASSERT_EQUAL_STRING("parm1-testdata", wifiParms2[0]->getValue());

  TEST_ASSERT_EQUAL_STRING("parm2id", wifiParms2[1]->getID());
  TEST_ASSERT_EQUAL_STRING("parm2-testdata", wifiParms2[1]->getPlaceholder());
  TEST_ASSERT_EQUAL_STRING("parm2-testdata", wifiParms2[1]->getValue());
}
void setup() {
    UNITY_BEGIN();

    RUN_TEST(test_WifiManager_parameters);
    RUN_TEST(test_WifiManager_testCtor);
    RUN_TEST(test_MetricTemp_getJson);
    RUN_TEST(test_Elasticsearch_getFullURL);
    RUN_TEST(test_Elasticsearch_indexRecord);
    RUN_TEST(test_SensorTemp_readSensor);
    RUN_TEST(test_Filesystem_testKV);

    UNITY_END();
}

void loop() {
}

