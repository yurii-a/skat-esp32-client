#include <Arduino.h>
#include <SPIFFS.h>
#include <FS.h>
#include <ArduinoJson.h>
#include <WiFi.h>
#include "config/save_config.h"

#define FILE_MODE_R "r"

const unsigned long timeout = 10000; 

bool setup_wifi() {
  String ssid;
  String password;

  get_wifi_config(&ssid, &password);

  Serial.println("\n\n----- WiFi Setup -----");
  WiFi.begin(ssid, password);

  unsigned long startAttemptTime = millis();

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");

    if (millis() - startAttemptTime > timeout) {
      Serial.println("\nWiFi connection timed out!");
      // Handle connection failure, e.g., retry or enter deep sleep
      break;
    }
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\nWiFi connected: " + ssid);
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("\nWiFi connection failed!");
    // Handle connection failure, e.g., try again or report error
    return false;
  }

  return true;
}