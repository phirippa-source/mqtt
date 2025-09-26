#include <WiFi.h>
#include <PubSubClient.h>
#include "DHT.h"
#include <Wire.h>
#include <BH1750.h>

#define DHTPIN D4
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);
BH1750 lightMeter;
const char* ssid = "Ria2G";
const char* password = "730124go";
const char* serverIPAddr = "192.168.0.6";
const char* userId = "ship";
const char* userPw = "1234";
const char* clientId = "1234xx";
const char *topic = "MyOffice/Indoor/Value";
const int PAYLOADSIZE = 128;

void setup() {
  dht.begin();
  Wire.begin();
  lightMeter.begin();
  Serial.begin(9600);
  Serial.print("\nConnecting to "); Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print("."); delay(500);
  }
  Serial.println("\nWiFi Connected");

  Serial.print("Connecting to broker(" + String(serverIPAddr) + ")");
  while ( !client.connect(clientId, userId, userPw) ){ 
    Serial.print("*"); delay(500);
  }
  Serial.println("\nConnected to broker!!!");
}

void loop() {
  char payload[PAYLOADSIZE];
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  if (isnan(h) || isnan(t) ) {
    Serial.println("Failed to read from DHT sensor!");  return;
  }
  float lux = lightMeter.readLightLevel();
  String sPayload = "{\"Temp\":" + String(t, 1)
                  + ",\"Humi\":" + String(h, 1)
                  +",\"Lux\":"   + String(lux, 1) + "}";
  sPayload.toCharArray(payload, PAYLOADSIZE);
  client.publish(topic, payload);
  Serial.print(String(topic) + " "); 
  Serial.println(payload);
  delay(1000);
}
