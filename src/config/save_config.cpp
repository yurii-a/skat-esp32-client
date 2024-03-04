#include <Arduino.h>
#include <SPIFFS.h>
#include <FS.h>
#include <ArduinoJson.h>

#define FILE_MODE_W "w"
#define FILE_MODE_R "r"

bool getWifiConfig(String *ssid, String *password)
{
  if (!SPIFFS.begin(true))
  {
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

  const char *_ssid = doc["ssid"];
  const char *_password = doc["password"];

  *ssid = _ssid;
  *password = _password;

  readConfigFile.close();

  return true;
}

bool getPrivateKey(unsigned char *pk)
{
  if (!SPIFFS.begin(true))
  {
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
  if (error)
  {
    Serial.print(F("ERROR: "));
    Serial.println(error.f_str());
    return false;
  }

  // Get private_key array
  JsonArray private_key = doc["private_key"];

  if (private_key.size() != 64)
  {
    Serial.println("ERROR: Invalid private key length, expected 64 bytes");
    return false;
  }

  // Convert to unsigned char array
  for (size_t i = 0; i < private_key.size(); i++)
  {
    pk[i] = static_cast<unsigned char>(private_key[i]);
  }

  readConfigFile.close();

  return true;
}

bool saveConfig()
{
  if (!SPIFFS.begin(true))
  {
    Serial.println("ERROR: Failed to mount file system");
    return false;
  }

  File configFile = SPIFFS.open("/config.txt", FILE_MODE_W);
  if (!configFile)
  {
    Serial.println("Failed to open config.txt for writing");
    return false;
  }

  configFile.println("{ \"ssid\": \"<SSID>\", \"password\": \"<PASSWORD>\", \"private_key\": <PRIVATE_KEY> }");
  configFile.close();

  SPIFFS.end();

  Serial.println("Config saved!");

  String ssid;
  String password;

  getWifiConfig(&ssid, &password);

  Serial.println("ssid: " + ssid);

  return true;
}
