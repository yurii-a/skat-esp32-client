#include <Arduino.h>
#include <SPIFFS.h>
#include <FS.h>
#include <LittleFS.h>
#include <ArduinoJson.h>

bool getWifiConfig(String* ssid, String* password);

bool getPrivateKey(unsigned char* privateKey);

bool saveConfig();

