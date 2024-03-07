#include <Arduino.h>
#include <SPIFFS.h>
#include <FS.h>
#include <ArduinoJson.h>
#include <WiFi.h>
#include "config/save_config.h"
#include "server/web_server.h"

#define FILE_MODE_R "r"

const unsigned long timeout = 10000;

bool setupWifi()
{
  String ssid;
  String password;

  getWifiConfig(&ssid, &password);

  Serial.println("\n\n----- WiFi Setup -----");
  WiFi.begin(ssid, password);

  unsigned long startAttemptTime = millis();

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");

    if (millis() - startAttemptTime > timeout)
    {
      Serial.println("\nWiFi connection timed out!");
      // Handle connection failure, e.g., retry or enter deep sleep
      break;
    }
  }

  if (WiFi.status() == WL_CONNECTED)
  {
    Serial.println("\nWiFi connected: " + ssid);
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
  }
  else
  {
    WiFi.disconnect();
    Serial.println("\nWiFi connection failed!");
    // Handle connection failure, e.g., try again or report error
    // return false;
  }
  // setupWebServer();

  return true;
}