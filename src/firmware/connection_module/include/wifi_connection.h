#ifndef WIFI_CONNECTION_H
#define WIFI_CONNECTION_H

#include <Arduino.h>
#include <WiFi.h>

#define WIFI_LED 32

void connect(String ssid, String pass) {
  WiFi.begin(ssid, pass, 6);
}

void verifyWifiConnection() {
  if (WiFi.status() == WL_CONNECTED) {
    digitalWrite(WIFI_LED, HIGH);
    return;
  }

  if (millis() - wifiMillis >= 500) {
    wifiMillis = millis();
    blinkWifiLed();
  }
}

#endif