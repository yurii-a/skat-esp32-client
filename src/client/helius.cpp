#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

const char* devnetRPC = "https://kora-disz8d-fast-devnet.helius-rpc.com/";
const char* mainnetRPC = "https://cecily-q1u5dh-fast-mainnet.helius-rpc.com/";

bool rpc() {
  // Establish WiFi connection (code from previous snippet)

  // Make the REST API call
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    // http.begin(mainnetRPC);
    http.begin(devnetRPC);
    int httpResponseCode = http.POST("{\"id\":34,\"jsonrpc\":\"2.0\",\"method\":\"getEpochInfo\",\"params\":[]}");

    if (httpResponseCode > 0) {
      String payload = http.getString();
      // Serial.println(payload);  // Print raw response

      // Deserialize JSON data
      DynamicJsonDocument doc(1024);  // Adjust capacity as needed
      deserializeJson(doc, payload);

      // Access parsed data
      int blockHeight = doc["result"]["blockHeight"];

      Serial.println("blockHeight: " + String(blockHeight));
    } else {
      Serial.print("HTTP error: ");
      Serial.println(httpResponseCode);
      return false;
    }

    http.end();
  }

  return true;
}