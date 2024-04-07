#include <Arduino.h>
#include "config/save_config.h"
#include "config/utils.h"
#include "wifi/setup_wifi.h"
#include "client/transfer.h"
#include "client/anchor.h"
#include "client/bonk.h"
#include "client/helius.h"
#include "SolanaSDK/keypair.h"
#include "server/web_server.h"
#include "Wire.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <qrcode.h>

void setup()
{
  Serial.begin(115200);

  setupWifi();
}

void loop()
{
  axsServer.handleClient();
}