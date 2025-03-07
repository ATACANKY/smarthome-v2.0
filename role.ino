#include <ESP8266WiFi.h>
#include <ArduinoJson.h>

// ...existing code...

void setup() {
  // ...existing code...
  WiFi.begin("SSID", "PASSWORD");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
  }
  // ...existing code...
}

void loop() {
  // ...existing code...
  if (WiFi.status() == WL_CONNECTED) {
    WiFiClient client;
    if (client.connect("your-server-ip", 80)) {
      client.print("GET /role_status.json HTTP/1.1\r\nHost: your-server-ip\r\nConnection: close\r\n\r\n");
      while (client.connected() || client.available()) {
        if (client.available()) {
          String line = client.readStringUntil('\r');
          StaticJsonDocument<200> doc;
          deserializeJson(doc, line);
          bool status = doc["status"];
          // Role kontrol kodu
          if (status) {
            // Role aç
          } else {
            // Role kapat
          }
        }
      }
      client.stop();
    }
  }
  delay(10000); // 10 saniye bekle
}
