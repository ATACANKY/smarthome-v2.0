#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>
#include <Adafruit_NeoPixel.h>

#define PIN 2
#define ROLE_PIN 4
#define NUMPIXELS 8

Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  Serial.begin(115200);
  pixels.begin();
  pixels.show(); // Initialize all pixels to 'off'
  pinMode(ROLE_PIN, OUTPUT);
  digitalWrite(ROLE_PIN, LOW); // Role başlangıçta kapalı

  WiFi.begin("SSID", "PASSWORD");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("WiFi bağlantısı bekleniyor...");
  }
  Serial.println("WiFi bağlantısı başarılı!");
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    WiFiClient client;

    http.begin(client, "http://youripadress/yourfileadress/role_status.json"); // Sunucu IP adresini güncelleyin
    int httpCode = http.GET();
    if (httpCode > 0) {
      String payload = http.getString();
      handleResponse(payload, "role");
    } else {
      Serial.printf("Role HTTP GET hatası: %s\n", http.errorToString(httpCode).c_str());
    }
    http.end();

    http.begin(client, "http://youripadress/yourfileadress/neopixel_status.json"); // Sunucu IP adresini güncelleyin
    httpCode = http.GET();
    if (httpCode > 0) {
      String payload = http.getString();
      handleResponse(payload, "neopixel");
    } else {
      Serial.printf("Neopixel HTTP GET hatası: %s\n", http.errorToString(httpCode).c_str());
    }
    http.end();
  } else {
    Serial.println("WiFi bağlantısı kaybedildi!");
  }
  delay(10000); // 10 saniye bekle
}

void handleResponse(String payload, const char* device) {
  StaticJsonDocument<200> doc;
  DeserializationError error = deserializeJson(doc, payload);
  if (error) {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.f_str());
    return;
  }
  bool status = doc["status"];
  Serial.println("JSON dosyasına girildi");
  if (strcmp(device, "role") == 0) {
    // Role kontrol kodu
    if (status) {
      digitalWrite(ROLE_PIN, HIGH); // Role aç
    } else {
      digitalWrite(ROLE_PIN, LOW); // Role kapat
    }
  } else if (strcmp(device, "neopixel") == 0) {
    const char* mode = doc["mode"];
    const char* color = doc["color"];
    // Neopixel kontrol kodu
    if (status) {
      if (mode && strcmp(mode, "rainbow") == 0) {
        rainbow(10);
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

void rainbow(int wait) {
  for(long firstPixelHue = 0; firstPixelHue < 5*65536; firstPixelHue += 256) {
    for(int i=0; i<NUMPIXELS; i++) { 
      int pixelHue = firstPixelHue + (i * 65536L / NUMPIXELS);
      pixels.setPixelColor(i, pixels.gamma32(pixels.ColorHSV(pixelHue)));
    }
    pixels.show();
    delay(wait);
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
