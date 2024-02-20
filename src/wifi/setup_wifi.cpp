#include <Arduino.h>
#include <SPIFFS.h>
#include <FS.h>
#include <ArduinoJson.h>
#include <WiFi.h>
#include "config/save_config.h"
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <WiFiManager.h>

#define FILE_MODE_R "r"

const unsigned long timeout = 10000; 

// AsyncWebServer server(80);

// void initWebServer() {
//   server.on("/", HTTP_POST, [](AsyncWebServerRequest *request) {
//     if (request->hasParam("public_key", true)) {
//       String pubkey = request->getParam("public_key", true)->value();

      
//       // Handle received credentials
//       Serial.println("Received" + pubkey);

//       request->send(200, "text/plain", "public key received successfully");
//     } else {
//       request->send(400, "text/plain", "Missing pubkey");
//     }
//   });

//   // Start server
//   server.begin();
// }


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

  // wifi manager and webserver is commented for convience of testing the lib

  // WiFi.mode(WIFI_STA);

  //   WiFiManager wm;
 
  //   // reset settings - wipe stored credentials for testing
  //   // these are stored by the esp library
  //   //wm.resetSettings();
  //   bool res;
  //   res = wm.autoConnect("AutoConnectAP"); // anonymous ap

 
  //   if(!res) {
  //       Serial.println("Failed to connect");
  //       // ESP.restart();
  //   } 
  //   else {
  //       //if you get here you have connected to the WiFi    
  //       Serial.println("connected...yeey :)");
  //       initWebServer();
  //   }

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