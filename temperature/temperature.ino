// ESP32 code to read DS18B20 temperature sensor data and post it to influxdb

#include <HTTPClient.h>
#include <WiFi.h>

#include <OneWire.h>
#include <DallasTemperature.h>

const char* ssid = "You're my moon";
const char* password = "thisisapassword";

char params[32];

// GPIO where the DS18B20 is connected to
const int oneWireBus = 4;

// Setup a oneWire instance to communicate with any OneWire devices
OneWire oneWire(oneWireBus);

// Pass our oneWire reference to Dallas Temperature sensor
DallasTemperature sensors(&oneWire);

void setup() {
  Serial.begin(115200);
  delay(4000);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }
  Serial.println("Connected to the WiFi network");

  sensors.begin();
}

void loop() {
  sensors.requestTemperatures();
  float temperatureC = sensors.getTempCByIndex(0);

  Serial.print(temperatureC);
  Serial.println("C");

  if ((WiFi.status() == WL_CONNECTED)) {

    HTTPClient influxdb;
    influxdb.begin("pikapika.fritz.box", 8086, "/write?db=fishtank");
    influxdb.addHeader("Content-Type", "text/plain");
    sprintf(params, "temp value=%f", temperatureC);
    int httpResponseCode = influxdb.POST(params);
    influxdb.end();

    delay(30000);

  } else {
    ESP.restart();
  }


}
