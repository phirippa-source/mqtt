#include <ESP8266WiFi.h>
const char* ssid = "RiatechChief_2G";
const char* password = "12345678";

void setup() {
  Serial.begin(9600);
 
  Serial.print("\nConnecting to ");
  Serial.println(ssid);

  // 해당 공유기에 접속 시도
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("\nWiFi Connected");
  
  // 공유기로부터 할당 받은 "(사설)IP 주소" 출력
  Serial.print("Local IP address : ");
  Serial.println(WiFi.localIP());
}

void loop() {
}
