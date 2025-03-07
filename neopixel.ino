#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
#include <Adafruit_NeoPixel.h>

#define PIN D1
#define NUMPIXELS 16

Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  pixels.begin();
  pixels.show(); // Initialize all pixels to 'off'

  WiFi.begin("SSID", "PASSWORD");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
  }
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    WiFiClient client;
    if (client.connect("your-server-ip", 80)) {
      client.print("GET /neopixel_status.json HTTP/1.1\r\nHost: your-server-ip\r\nConnection: close\r\n\r\n");
      while (client.connected() || client.available()) {
        if (client.available()) {
          String line = client.readStringUntil('\r');
          StaticJsonDocument<200> doc;
          deserializeJson(doc, line);
          bool status = doc["status"];
          const char* mode = doc["mode"];
          const char* color = doc["color"];
          // Neopixel kontrol kodu
          if (status) {
            if (mode && strcmp(mode, "rainbow") == 0) {
              rainbow();
            } else if (color) {
              uint32_t rgb = strtol(color, NULL, 16);
              for (int i = 0; i < NUMPIXELS; i++) {
                pixels.setPixelColor(i, rgb);
              }
              pixels.show();
            }
          } else {
            for (int i = 0; i < NUMPIXELS; i++) {
              pixels.setPixelColor(i, pixels.Color(0, 0, 0)); // Turn off
            }
            pixels.show();
          }
        }
      }
      client.stop();
    }
  }
  delay(10000); // 10 saniye bekle
}

void rainbow() {
  uint16_t i, j;
  for (j = 0; j < 256; j++) {
    for (i = 0; i < NUMPIXELS; i++) {
      pixels.setPixelColor(i, Wheel((i + j) & 255));
    }
    pixels.show();
    delay(20);
  }
}

uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if (WheelPos < 85) {
    return pixels.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else if (WheelPos < 170) {
    WheelPos -= 85;
    return pixels.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  } else {
    WheelPos -= 170;
    return pixels.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  }
}
