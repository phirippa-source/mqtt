#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#include <Wire.h>
#include <BH1750.h>
BH1750 lightMeter;

const char* ssid = "RiatechChief_2G";
const char* password = "12345678";

const char* userId = "mqtt_phirippa";
const char* userPw = "1234";
const char* clientId = userId;
char *topic = "Sensors/MyOffice/Indoor/Lux";
char* server = "192.168.1.12"; 
WiFiClient wifiClient; 
PubSubClient client(server, 1883, wifiClient);

void setup() {
  Serial.begin(9600);
  Serial.print("\nConnecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("\nWiFi Connected");
  Serial.print("Local IP address : ");
  Serial.println(WiFi.localIP());

  Serial.println("Connecting to broker");
  while ( !client.connect(clientId, userId, userPw) ){ 
    Serial.print("*");
    delay(1000);
  }
  Serial.println("\nConnected to broker");
  Wire.begin();
  lightMeter.begin();
}

void loop() {
  char buf[20];
  String strLuxValue = String( lightMeter.readLightLevel() );
  strLuxValue.toCharArray(buf, strLuxValue.length() );
  
  client.publish(topic, buf);
  Serial.println(String(topic) + " : " + buf);
  delay(2000);
}
