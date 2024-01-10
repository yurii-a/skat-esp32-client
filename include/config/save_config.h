#include <Arduino.h>
#include <SPIFFS.h>
#include <FS.h>
#include <LittleFS.h>
#include <ArduinoJson.h>

bool get_wifi_config(String* ssid, String* password);

bool get_private_key(unsigned char* private_key);

bool save_config();
