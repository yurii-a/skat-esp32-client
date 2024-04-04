#include <Arduino.h>
#include <SPIFFS.h>
#include <FS.h>
#include <LittleFS.h>
#include <ArduinoJson.h>

bool mountFileSystem();

bool unmountFileSystem();

bool readFile(const char* filePath, String& content);

bool writeFile(const char* filePath, const String& content);

bool getWifiConfig(String& ssid, String& password);

bool saveWifiConfig(const String& ssid, const String& password);

bool getPrivateKey(String &privateKey);

bool savePrivateKey(const String &privateKey);

bool saveConfig(const String &ssid, const String &password, const String &privateKey);