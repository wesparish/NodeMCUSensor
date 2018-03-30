#include <FS.h>

#include <ESP8266WiFi.h>          //https://github.com/esp8266/Arduino

//needed for library
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>          //https://github.com/tzapu/WiFiManager

#include <ESP8266HTTPClient.h>
#include <time.h>
#include <ArduinoJson.h>          //https://github.com/bblanchon/ArduinoJson

//for LED status
#include <Ticker.h>
Ticker ticker;

// For NTP
#include <NtpClientLib.h>
//#define ARDUINO_ARCH_ESP8266

#include <Adafruit_Sensor.h>
//for DHT22
#include <DHT.h>
#define DHTPIN 5     // what digital pin the DHT22 is conected to
#define DHTTYPE DHT22   // there are multiple kinds of DHT sensors

#define SWITCHPIN 4

// GLOBALS
#define TEMP_READ_INTERVAL 10
char elasticsearch_url[40] = "elasticsearch.weshouse:9200";
char location[40] = "weshouse";
bool ntpSynced = false;

void tick()
{
  //toggle state
  int state = digitalRead(BUILTIN_LED);  // get the current state of GPIO1 pin
  digitalWrite(BUILTIN_LED, !state);     // set pin to the opposite state
}

// Make sure we have NTP sync before sending data
void processSyncEvent (NTPSyncEvent_t ntpEvent) {
    if (ntpEvent) {
        Serial.print ("Time Sync error: ");
        if (ntpEvent == noResponse)
            Serial.println ("NTP server not reachable");
        else if (ntpEvent == invalidAddress)
            Serial.println ("Invalid NTP server address");
    } else {
        Serial.print ("Got NTP time: ");
        Serial.println (NTP.getTimeDateString (NTP.getLastNTPSync ()));
        ntpSynced = true;
    }
}

bool okToReadFromDHT22 = false;
void readDHT22()
{
  okToReadFromDHT22 = false;
  DHT dht(DHTPIN, DHTTYPE);
  float h = dht.readHumidity();
  float f = dht.readTemperature(true);

  if (isnan(h) || isnan(f))
  {
    Serial.println("Failed to read from DHT sensor or NTP not synced!");
  }
  else
  {
    // Compute heat index
    float hif = dht.computeHeatIndex(f, h);
    Serial.print("Humidity: ");
    Serial.print(h);
    Serial.print(" %\t");
    Serial.print(f);
    Serial.print(" *F\t");
    Serial.print("Heat index: ");
    Serial.print(hif);
    Serial.println(" *F");

    //getDateStr() returns: 03/03/2018
    //getTimeDateString() return: 07:15:12 03/03/2018
    int hour=0, minute=0, second=0, month=0, day=0, year=0;
    sscanf(NTP.getTimeDateString().c_str(),
           "%02d:%02d:%02d %02d/%02d/%04d",
           &hour, &minute, &second,
           &day, &month, &year);

    int httpCode = -1;
    char fullUrl[128] = {0};
    sprintf(fullUrl,
            "http://elasticsearch.weshouse:9200/tempsensor-%s-%04d.%02d.%02d/doc",
            location,
            year, month, day);
    Serial.println(fullUrl);
    HTTPClient http;
    http.begin(fullUrl);

    char timestamp[64] = {0};
    sprintf(timestamp,
            "%04d-%02d-%02dT%02d:%02d:%02dZ",
            year, month, day,
            hour, minute, second);
    DynamicJsonBuffer jsonBuffer;
    JsonObject& json = jsonBuffer.createObject();
    json["location"] = location;
    json["temp"] = f;
    json["humidity"] = h;
    json["heat_index"] = hif;
    json["@timestamp"] = timestamp;

    http.addHeader("Content-Type", String("application/json"));

    char jsonHTTPBuffer[128];
    json.printTo(jsonHTTPBuffer);
    httpCode = http.POST(jsonHTTPBuffer);
    Serial.printf("[%d] %s\n", httpCode, http.getString().c_str());
    Serial.printf("NTP.getTimeDateString: %s\n", NTP.getTimeDateString().c_str());
//    Serial.printf("NTP.getDateString: %s", NTP.getDateStr().c_str());
    http.end();
  }

  return;
}

void setOkToReadFromDHT22()
{
  okToReadFromDHT22 = true;
}

//gets called when WiFiManager enters configuration mode
void configModeCallback (WiFiManager *myWiFiManager) {
  Serial.println("Entered config mode");
  Serial.println(WiFi.softAPIP());
  //if you used auto generated SSID, print it
  Serial.println(myWiFiManager->getConfigPortalSSID());
  //entered config mode, make led toggle faster
  ticker.attach(0.2, tick);
}

bool shouldSaveConfig = false;

//callback notifying us of the need to save config
void saveConfigCallback () {
  Serial.println("Should save config");
  shouldSaveConfig = true;
}

void loadFromFS()
{
  // If we fail to mount, format it first
  if (!SPIFFS.begin())
  {
    if (!SPIFFS.format())
    {
      Serial.println("FAILED to format SPIFFS");
    }
  }

  Serial.println("mounting FS...");
  if (SPIFFS.begin()) {
    Serial.println("mounted file system");
    if (SPIFFS.exists("/config.json")) {
      //file exists, reading and loading
      Serial.println("reading config file");
      File configFile = SPIFFS.open("/config.json", "r");
      if (configFile) {
        Serial.println("opened config file");
        size_t size = configFile.size();
        // Allocate a buffer to store contents of the file.
        std::unique_ptr<char[]> buf(new char[size]);

        configFile.readBytes(buf.get(), size);
        configFile.close();
        DynamicJsonBuffer jsonBuffer;
        JsonObject& json = jsonBuffer.parseObject(buf.get());
        json.printTo(Serial);
        if (json.success()) {
          Serial.println("\nparsed json");

          strcpy(elasticsearch_url, json["elasticsearch_url"]);
          strcpy(location, json["location"]);

        } else {
          Serial.println("failed to load json config");
        }
      }
    }
  } else {
    Serial.println("failed to mount FS");
  }
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  //set led pin as output
  pinMode(BUILTIN_LED, OUTPUT);
  pinMode(SWITCHPIN, INPUT_PULLUP);
  //digitalWrite(SWITCHPIN, HIGH);
  // start ticker with 0.5 because we start in AP mode and try to connect
  ticker.attach(0.6, tick);

  loadFromFS();

  WiFiManagerParameter custom_elasticsearch_url("elasticsearch_url", "elasticsearch.weshouse:9200", elasticsearch_url, 40);
  WiFiManagerParameter custom_location("location", "weshouse", location, 40);

  //WiFiManager
  //Local intialization. Once its business is done, there is no need to keep it around
  WiFiManager wifiManager;
  //reset settings - for testing

  if (!digitalRead(SWITCHPIN))
  {
    wifiManager.resetSettings();
  }

  wifiManager.addParameter(&custom_elasticsearch_url);
  wifiManager.addParameter(&custom_location);

  //set callback that gets called when connecting to previous WiFi fails, and enters Access Point mode
  wifiManager.setAPCallback(configModeCallback);
  wifiManager.setSaveConfigCallback(saveConfigCallback);

  //fetches ssid and pass and tries to connect
  //if it does not connect it starts an access point with the specified name
  //here  "AutoConnectAP"
  //and goes into a blocking loop awaiting configuration
  if (!wifiManager.autoConnect()) {
    Serial.println("failed to connect and hit timeout");
    //reset and try again, or maybe put it to deep sleep
    ESP.reset();
    delay(1000);
  }

  //if you get here you have connected to the WiFi
  Serial.println("connected to wifi");
  ticker.detach();

  if (shouldSaveConfig)
  {
    // Update defaults
    strcpy(elasticsearch_url, custom_elasticsearch_url.getValue());
    strcpy(location, custom_location.getValue());

    Serial.println("saving config to fs");
    DynamicJsonBuffer jsonBuffer;
    JsonObject& json = jsonBuffer.createObject();
    json["elasticsearch_url"] = elasticsearch_url;
    json["location"] = location;

    File configFile = SPIFFS.open("/config.json", "w");
    if (!configFile) {
      Serial.println("failed to open config file for writing");
    }

    json.printTo(Serial);
    json.printTo(configFile);
    configFile.close();
  }

  // Setup NTP
  //NTP.begin("pool.ntp.org", -6, true, 0);
  NTP.begin();
  NTP.onNTPSyncEvent ([](NTPSyncEvent_t event) {
    processSyncEvent (event);
    });

  // Setup temp read ticker
  ticker.attach(TEMP_READ_INTERVAL, setOkToReadFromDHT22);

  //keep LED on
  digitalWrite(BUILTIN_LED, LOW);
  okToReadFromDHT22 = true;
}

void loop() {
  // put your main code here, to run repeatedly:
  if (okToReadFromDHT22)
  {
    readDHT22();
  }

  delay(1);
}

