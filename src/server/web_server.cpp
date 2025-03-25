#include "web_server.h"
#include "config/save_config.h"
#include "client/anchor.h"
#include "client/skat.h"
#include "client/vote_ix.h"
#include "client/stake_ix.h"
#include "SolanaSDK/keypair.h"
#include "SolanaSDK/base58.h"
#include "config/utils.h"
#include <Update.h> // Include the Update library for OTA updates

WebServer server(80);

const char *softAP_ssid = "Magic Gadget";
const char *softAP_password = "1234567890";

void addCorsHeaders()
{
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.sendHeader("Access-Control-Allow-Methods", "GET,POST,PUT,PATCH,DELETE,OPTIONS");
  server.sendHeader("Access-Control-Allow-Headers", "Content-Type, Access-Control-Allow-Headers, Authorization, X-Requested-With");
}

void handleSetup()
{
  addCorsHeaders();
  String html = R"rawliteral(
<!DOCTYPE HTML>
<html>
<head>
<title>Connect to Wifi</title>
<meta name="viewport" content="width=device-width, initial-scale=1">
<style>
body {
    color: white;
    background-color: #141414;
    font-family: Arial, sans-serif;
}
h1 {
    text-align: center;
    color: #333;
}
form {
    margin-top: 20px;
}
input[type='text'], input[type='password'] {
    padding: 5px;
    margin-bottom: 10px;
}
input[type='submit'] {
    padding: 8px 12px;
    background-color: #4CAF50;
    color: white;
    border: none;
}
</style>
</head>
<body>
<h1>Let's get started</h1>
<h2>Enter WiFi details to connect your device to Internet</h2>
<form id="configForm">
  SSID: <input type='text' id='ssid' name='ssid'><br>
  Password: <input type='password' id='password' name='password'><br>
  <input type='submit' value='Confirm & Continue'>
</form>
<script>
document.getElementById('configForm').addEventListener('submit', function(event) {
  event.preventDefault();
  var ssid = document.getElementById('ssid').value;
  var password = document.getElementById('password').value;
  fetch('/submit?ssid=' + ssid + '&password=' + password)
    .then(response => {
      if (response.ok) {
        console.log('Submitted successfully');
      } else {
        console.error('Submission failed');
      }
    })
    .catch(error => {
      console.error('Error submitting:', error);
    });
});
</script>
</body>
</html>)rawliteral";

  server.send(200, "text/html", html);
}

void handleSubmit()
{
  addCorsHeaders();
  String ssid = server.arg("ssid");
  String password = server.arg("password");

  Serial.print("SSID: ");
  Serial.println(ssid);

  Keypair keypair = Keypair::generate();
  String privateKey = keypairToString(keypair).c_str();

  Serial.print("The Solana keypair is initialized: ");
  Serial.println(keypair.publicKey.toBase58().c_str());

  saveConfig(ssid, password, privateKey);
  StaticJsonDocument<128> response;
  response["publicKey"] = keypair.publicKey.toBase58();

  String responsePayload;
  serializeJson(response, responsePayload);

  server.send(200, "application/json", responsePayload);
  // ESP.restart();
}

void handleEdit()
{
  addCorsHeaders();
  String html = R"rawliteral(
<!DOCTYPE HTML>
<html>
<head>
<title>AXS Dashboard</title>
<meta name="viewport" content="width=device-width, initial-scale=1">
<style>
body {
    color: white;
    background-color: #141414;
    font-family: Arial, sans-serif;
}
h1 {
    text-align: center;
    color: #333;
}
form {
    margin-top: 20px;
}
input[type='text'], input[type='password'] {
    padding: 5px;
    margin-bottom: 10px;
}
input[type='submit'] {
    padding: 8px 12px;
    background-color: #4CAF50;
    color: white;
    border: none;
}
</style>
</head>
<body>
<h1>AXS Dashboard</h1>
<form id="configForm">
  SSID: <input type='text' id='ssid' name='ssid'><br>
  Password: <input type='password' id='password' name='password'><br>
  <input type='submit' value='Update'>
</form>
<script>
document.getElementById('configForm').addEventListener('submit', function(event) {
  event.preventDefault();
  var ssid = document.getElementById('ssid').value;
  var password = document.getElementById('password').value;
  fetch('/update?ssid=' + ssid + '&password=' + password)
    .then(response => {
      if (response.ok) {
        console.log('Updated successfully');
      } else {
        console.error('Update failed');
      }
    })
    .catch(error => {
      console.error('Error updating:', error);
    });
});
</script>
</body>
</html>)rawliteral";

  server.send(200, "text/html", html);
}

void handleUpdate()
{
  addCorsHeaders();
  String ssid = server.arg("ssid");
  String password = server.arg("password");

  Serial.print("SSID: ");
  Serial.println(ssid);

  String configSecretKey;

  getPrivateKey(configSecretKey);
  Keypair keypair = stringToKeypair(configSecretKey.c_str());

  Serial.print("PUBLIC KEY: ");
  Serial.println(keypair.publicKey.toBase58().c_str());

  saveWifiConfig(ssid, password);
  ESP.restart();
}

void invokeAnchor()
{
  addCorsHeaders();
  String data = server.arg("name");
  uint8_t dataHex[data.length()];
  int dataHexSize;

  stringToHexArray(data, dataHex, dataHexSize);
  Serial.print("data parameter value (hex): ");
  for (int i = 0; i < dataHexSize; i++)
  {
    Serial.print(dataHex[i], HEX);
    Serial.print(" ");
  }
  Serial.println();

  Serial.print("Size of dataHex array: ");
  Serial.println(dataHexSize);

  String signature = anchor(data, dataHexSize);

  StaticJsonDocument<128> response;
  response["signature"] = signature;

  String responsePayload;
  serializeJson(response, responsePayload);

  server.send(200, "application/json", responsePayload);
}

void invokeSendBid()
{
  addCorsHeaders();
  String data = "";//axsServer.arg("name");
  uint8_t dataHex[data.length()];
  int dataHexSize;

  stringToHexArray(data, dataHex, dataHexSize);
  Serial.print("data parameter value (hex): ");
  for (int i = 0; i < dataHexSize; i++)
  {
    Serial.print(dataHex[i], HEX);
    Serial.print(" ");
  }
  Serial.println();

  Serial.print("Size of dataHex array: ");
  Serial.println(dataHexSize);

  String signature = anchor(data, dataHexSize);

  StaticJsonDocument<128> response;
  response["signature"] = signature;

  String responsePayload;
  serializeJson(response, responsePayload);

  server.send(200, "application/json", responsePayload);
}


void invokeVote()
{
  addCorsHeaders();
  String data = server.arg("cast");

  int cast = atoi(data.c_str());

  String signature;
  if (cast == 0)
  {
    signature = voteIx(1);
  }
  else
  {
    signature = voteIx(2);
  }

  StaticJsonDocument<128> response;
  response["signature"] = signature;

  String responsePayload;
  serializeJson(response, responsePayload);

  server.send(200, "application/json", responsePayload);
}

void invokeStake()
{
  addCorsHeaders();
  String combinedParam = server.arg("type");

  String type = combinedParam.substring(0, combinedParam.indexOf('?'));
  String amount = combinedParam.substring(combinedParam.indexOf('=') + 1);

  uint8_t amountInt = atoi(amount.c_str());

  String signature;
  if (type == "stake")
  {
    signature = stakeIx(0, amountInt);
  }
  else
  {
    signature = stakeIx(1, amountInt);
  }

  StaticJsonDocument<128> response;
  response["signature"] = signature;

  String responsePayload;
  serializeJson(response, responsePayload);

  server.send(200, "application/json", responsePayload);
}

void resetConfig()
{
  Serial.println("RESETTING WIFI CONFIG & PRIVATE KEY");
  addCorsHeaders();
  saveConfig("", "", "");
  server.send(200, "application/json", "Reset successfully!");
}

void resetWifiConfig()
{
  addCorsHeaders();
  saveWifiConfig("", "");
  server.send(200, "application/json", "Wifi reset successfully!");
}

void resetWalletConfig()
{
  addCorsHeaders();
  savePrivateKey("");

  Keypair keypair = Keypair::generate();
  String privateKey = keypairToString(keypair).c_str();

  Serial.print("PUBLIC_KEY: ");
  Serial.print(keypair.publicKey.toBase58().c_str());
  Serial.println("");

  savePrivateKey(privateKey);

  server.send(200, "application/json", "Wallet reset successfully!");
}

void setupWebServer()
{
  // Set up SoftAP
  WiFi.softAP(softAP_ssid, softAP_password);
  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);

  String configSecretKey;

  getPrivateKey(configSecretKey);

  bool isServerSetup = configSecretKey.length() == 0;

  // Start web server
  if (!isServerSetup)
  {
    server.on("/", HTTP_GET, handleSetup);
    Keypair keypair = stringToKeypair(configSecretKey.c_str());
    Serial.print("PUBLIC KEY: ");
    Serial.println(keypair.publicKey.toBase58().c_str());
  }
  else
  {
    server.on("/", HTTP_GET, handleEdit);
  }
  server.on("/submit", HTTP_GET, handleSubmit);
  server.on("/update", HTTP_GET, handleUpdate);
  server.on("/anchor", HTTP_GET, invokeAnchor);
  server.on("/vote", HTTP_GET, invokeVote);
  server.on("/stake", HTTP_GET, invokeStake);
  server.on("/reset", HTTP_GET, resetConfig);
  server.on("/reset-wifi", HTTP_GET, resetWifiConfig);
  server.on("/reset-wallet", HTTP_GET, resetWalletConfig);
  server.on("/sendBid", HTTP_GET, invokeSendBid);

  server.begin();
  Serial.println("Web server started");
}