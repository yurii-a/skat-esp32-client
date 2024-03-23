#include "web_server.h"
#include "config/save_config.h"
#include "client/anchor.h"
#include "SolanaSDK/keypair.h"

WebServer axsServer(80);

const char *softAP_ssid = "PROJECT-AXS";
const char *softAP_password = "axssolana";

std::string keypairToString(Keypair keypair)
{
  const unsigned char *k = keypair.getSecretKey();

  std::stringstream ss;
  ss << "[";
  for (int i = 0; k[i] != '\0' && i < SECRET_KEY_LEN; i++)
  {
    if (i > 0)
    {
      ss << ",";
    }
    ss << static_cast<int>(k[i]);
  }
  ss << "]";

  return ss.str();
}

void addCorsHeaders()
{
  axsServer.sendHeader("Access-Control-Allow-Origin", "*");
  axsServer.sendHeader("Access-Control-Allow-Methods", "GET,POST,PUT,PATCH,DELETE,OPTIONS");
  axsServer.sendHeader("Access-Control-Allow-Headers", "Content-Type, Access-Control-Allow-Headers, Authorization, X-Requested-With");
}

void handleRoot()
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
  <input type='submit' value='Submit'>
</form>
<script>
document.getElementById('configForm').addEventListener('submit', function(event) {
  event.preventDefault();
  var ssid = document.getElementById('ssid').value;
  var password = document.getElementById('password').value;
  fetch('/submit?ssid=' + ssid + '&password=' + password)
    .then(response => {
      if (response.ok) {
        console.log('Form submitted successfully');
      } else {
        console.error('Form submission failed');
      }
    })
    .catch(error => {
      console.error('Error submitting form:', error);
    });
});
</script>
</body>
</html>)rawliteral";

  axsServer.send(200, "text/html", html);
}

void handleSubmit()
{
  addCorsHeaders();
  String ssid = axsServer.arg("ssid");
  String password = axsServer.arg("password");

  Serial.print("SSID: ");
  Serial.println(ssid);

  Keypair keypair = Keypair::generate();
  String privateKey = keypairToString(keypair).c_str();

  Serial.print("PUBLIC_KEY: ");
  Serial.print(keypair.publicKey.toBase58().c_str());
  Serial.println("");

  saveConfig(ssid, password, privateKey);
  ESP.restart();
}

void invokedHelloWorldAnchor()
{
  addCorsHeaders();
  String signature = anchor();

  StaticJsonDocument<128> response;
  response["signature"] = signature;

  String responsePayload;
  serializeJson(response, responsePayload);

  axsServer.send(200, "application/json", responsePayload);
}

void resetConfig()
{
  addCorsHeaders();
  saveConfig("", "", "");
  axsServer.send(200, "application/json", "Reset successfully!");
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
  axsServer.on("/hello-world", HTTP_GET, invokedHelloWorldAnchor);
  axsServer.on("/reset", HTTP_GET, resetConfig);
  axsServer.begin();
  Serial.println("Web server started");
}
