#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include "DHT.h"
#include <Wire.h>
#include <BH1750.h>
#define USERSW  D8
#define DHTPIN  D4
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);
BH1750 lightMeter;

const char* ssid = "Ria2G";
const char* password = "730124go";
const char* serverIPAddr = "192.168.0.6";   // MQTT Broker가 실행 중인 컴퓨터의 IP address
const char* userId = "ship";                // MQTT Broker에 접속하기 위한 id와 password
const char* userPw = "1234";
const char* clientId = "1234xx2312";        // Client ID는 Broker에 접속한 클라이언트를 구분하기 위한 id. 유니크해야 함.
const char* topic = "MyOffice/Indoor/Value";
const int PAYLOADSIZE = 128;

WiFiClient wifiClient; 
PubSubClient client(serverIPAddr, 1883, wifiClient);

void setup() {
  dht.begin();
  Wire.begin();
  lightMeter.begin();
  Serial.begin(9600);
  pinMode(USERSW, INPUT);

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
  Serial.println("\nConnected to broker!!!\n");
}

void loop() {
  char payload[PAYLOADSIZE];
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  if (isnan(h) || isnan(t) ) {
    Serial.println("Failed to read from DHT sensor!");  
    return;
  }
  float lux = lightMeter.readLightLevel();
  int vol = analogRead(A0);
  int sw  = digitalRead(USERSW);

  String sPayload = "{\"Temp\":" + String(t, 1)
                  + ",\"Humi\":" + String(h, 1)
                  + ",\"Lux\":"  + String(lux, 1)
                  + ",\"Sw\":"   + String(sw)
                  + ",\"Vol\":"  + String(vol)
                  + "}";

  sPayload.toCharArray(payload, PAYLOADSIZE);
  client.publish(topic, payload);
  Serial.println( String(topic) + " : " + String(payload) ); 
  delay(1000);
}
