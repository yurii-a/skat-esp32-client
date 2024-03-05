#include "web_server.h"
#include "config/save_config.h"
#include "client/anchor.h"

WebServer axsServer(80);

const char *softAP_ssid = "ESP32-AP";
const char *softAP_password = "password";

void handleRoot()
{
  String html = "<!DOCTYPE html><html><head><title>ESP32 Form</title>";
  html += "<style>";
  html += "body { font-family: Arial, sans-serif; }";
  html += "h1 { color: #333; }";
  html += "form { margin-top: 20px; }";
  html += "input[type='text'] { padding: 5px; margin-bottom: 10px; }";
  html += "input[type='submit'] { padding: 8px 12px; background-color: #4CAF50; color: white; border: none; }";
  html += "</style>";
  html += "</head><body>";
  html += "<h1>AXS Dashboard</h1>";
  html += "<form action='/submit' method='get'>";
  html += "SSID: <input type='text' name='ssid'><br>";
  html += "Password: <input type='text' name='password'><br>";
  html += "Private Key: <input type='text' name='privateKey'><br>";
  html += "<input type='submit' value='Submit'></form>";
  html += "</body></html>";

  axsServer.send(200, "text/html", html);
}

void handleSubmit()
{
  String ssid = axsServer.arg("ssid");
  String password = axsServer.arg("password");
  String privateKey = axsServer.arg("privateKey");

  Serial.print("SSID: ");
  Serial.println(ssid);
  Serial.print("PASSWORD: ");
  Serial.println(password);
  Serial.print("PRIVATE_KEY: ");
  Serial.println(privateKey);

  saveConfig(ssid, password, privateKey);

  delay(5000);

  ESP.restart();
}

void invokeAnchor()
{

  String html = "<!DOCTYPE html><html><head><title>ESP32 Form</title>";
  html += "<style>";
  html += "body { font-family: Arial, sans-serif; }";
  html += "h1 { color: #333; }";
  html += "form { margin-top: 20px; }";
  html += "input[type='text'] { padding: 5px; margin-bottom: 10px; }";
  html += "input[type='submit'] { padding: 8px 12px; background-color: #4CAF50; color: white; border: none; }";
  html += "</style>";
  html += "</head><body>";
  html += "<h1>AXS Dashboard</h1>";
  html += "<form action='/invoked' method='get'>";
  html += "<input type='submit' value='Anchor Hello'></form>";
  html += "</body></html>";

  axsServer.send(200, "text/html", html);
}

void invokedAnchor()
{
  anchor();
  axsServer.send(200, "text/html", "Invoked successfully!");
}

void resetConfig()
{
  saveConfig("", "", "");
  axsServer.send(200, "text/html", "Reset successfully!");
  ESP.restart();
}

void setupWebServer()
{
  // Set up SoftAP
  WiFi.softAP(softAP_ssid, softAP_password);
  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);

  // Start web server
  axsServer.on("/", HTTP_GET, handleRoot);
  axsServer.on("/submit", HTTP_GET, handleSubmit);
  axsServer.on("/invoke", HTTP_GET, invokeAnchor);
  axsServer.on("/invoked", HTTP_GET, invokedAnchor);
  axsServer.on("/reset", HTTP_GET, resetConfig);
  axsServer.begin();
  Serial.println("Web server started");
}

void loop()
{
  axsServer.handleClient();
}
