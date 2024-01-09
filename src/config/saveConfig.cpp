#include <Arduino.h>
#include <SPIFFS.h>
#include <FS.h>
#include <ArduinoJson.h>

#define FILE_MODE_W "w"
#define FILE_MODE_R "r"

bool getWifiConfig(String* ssid, String* password) {
  if (!SPIFFS.begin(true)) {
    Serial.println("ERROR: Failed to mount file system");
    return false;
  }
  
  // Now open the file for reading.
  File readConfigFile = SPIFFS.open("/config.txt", FILE_MODE_R);

  // Serial.println("READ:" + readConfigFile.readString());

  DynamicJsonDocument doc(2048);
  String readConfigString = readConfigFile.readString();

  // Create a permanent char array
  char json[readConfigString.length() + 1];

  // Copy the contents of the String to the char array
  readConfigString.toCharArray(json, sizeof(json));

  // Now deserialize the char array
  deserializeJson(doc, json);

  const char* _ssid = doc["ssid"];
  const char* _password = doc["password"];

  *ssid = _ssid;
  *password = _password;

  readConfigFile.close();

  return true;
}

bool getPrivateKey(unsigned char* pk) {
  if (!SPIFFS.begin(true)) {
    Serial.println("ERROR: Failed to mount file system");
    return false;
  }
  
  // Now open the file for reading.
  File readConfigFile = SPIFFS.open("/config.txt", FILE_MODE_R);

  // Serial.println("READ:" + readConfigFile.readString());

  DynamicJsonDocument doc(2048);
  String readConfigString = readConfigFile.readString();

  // Create a permanent char array
  char json[readConfigString.length() + 1];

  // Copy the contents of the String to the char array
  readConfigString.toCharArray(json, sizeof(json));

  // Now deserialize the char array
  // Parse the JSON string
  DeserializationError error = deserializeJson(doc, json);

  // Test if parsing succeeds.
  if (error) {
    Serial.print(F("ERROR: "));
    Serial.println(error.f_str());
    return false;
  }

  // Get privateKey array
  JsonArray privateKey = doc["privateKey"];

  if (privateKey.size() != 64) {
    Serial.println("ERROR: Invalid private key length, expected 64 bytes");
    return false;
  }

  // Print privateKey to serial
  // for (int i = 0; i < privateKey.size(); i++) {
  //   Serial.println(privateKey[i].as<int>());
  // }

  // Convert to unsigned char array
  for (size_t i = 0; i < privateKey.size(); i++) {
      privateKey[i] = static_cast<unsigned char>(privateKey[i]);
  }

  readConfigFile.close();

  return true;
}

bool saveConfig() {
  if (!SPIFFS.begin(true)) {
    Serial.println("ERROR: Failed to mount file system");
    return false;
  }

  File configFile = SPIFFS.open("/config.txt", FILE_MODE_W);
  if (!configFile) {
    Serial.println("Failed to open config.txt for writing");
    return false;
  }
  
  configFile.println("{ \"ssid\": \"<SSID>\", \"password\": \"<PASSWORD>\", \"privateKey\": <PRIVATE_KEY> }");
  configFile.close();

  SPIFFS.end();

  Serial.println("Config saved!");

  String ssid;
  String password;

  getWifiConfig(&ssid, &password);

  Serial.println("ssid: " + ssid);
  Serial.println("password: " + password);

  return true;
}
