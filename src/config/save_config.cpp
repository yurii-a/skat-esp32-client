#include <Arduino.h>
#include <SPIFFS.h>
#include <FS.h>
#include <ArduinoJson.h>

#define FILE_MODE_W "w"
#define FILE_MODE_R "r"

const char* WIFI_CONFIG_FILE_PATH = "/wifi_config.txt";
const char* PRIVATE_KEY_FILE_PATH = "/private_key.txt";

bool mountFileSystem() {
  if (!SPIFFS.begin(true)) {
    Serial.println("ERROR: Failed to mount file system");
    return false;
  }
  return true;
}

bool unmountFileSystem() {
  SPIFFS.end();
  return true;
}

bool readFile(const char* filePath, String& content) {
  if (!mountFileSystem()) {
    return false;
  }

  File file = SPIFFS.open(filePath, FILE_MODE_R);
  if (!file) {
    Serial.println("ERROR: Failed to open file for reading");
    unmountFileSystem();
    return false;
  }

  content = file.readString();
  content.trim();
  file.close();
  unmountFileSystem();
  return true;
}

bool writeFile(const char* filePath, const String& content) {
  if (!mountFileSystem()) {
    return false;
  }

  File file = SPIFFS.open(filePath, FILE_MODE_W);
  if (!file) {
    Serial.println("ERROR: Failed to open file for writing");
    unmountFileSystem();
    return false;
  }
  
  file.println(content);
  file.close();
  unmountFileSystem();
  return true;
}

bool getWifiConfig(String& ssid, String& password) {
  String content;
  if (!readFile(WIFI_CONFIG_FILE_PATH, content)) {
    return false;
  }

  DynamicJsonDocument doc(1024);
  DeserializationError error = deserializeJson(doc, content);

  if (error) {
    Serial.println("ERROR: Failed to parse JSON");
    return false;
  }

  ssid = doc["ssid"].as<String>();
  password = doc["password"].as<String>();

  return true;
}

bool saveWifiConfig(const String& ssid, const String& password) {
  DynamicJsonDocument doc(1024);
  doc["ssid"] = ssid;
  doc["password"] = password;

  String content;
  serializeJson(doc, content);

  return writeFile(WIFI_CONFIG_FILE_PATH, content);
}

bool getPrivateKey(String& privateKey) {
  return readFile(PRIVATE_KEY_FILE_PATH, privateKey);
}

bool savePrivateKey(const String& privateKey) {
  return writeFile(PRIVATE_KEY_FILE_PATH, privateKey);
}

bool saveConfig(const String& ssid, const String& password, const String& privateKey) {
  if (!saveWifiConfig(ssid, password)) {
    return false;
  }
  return savePrivateKey(privateKey);
}
