#include <Arduino.h>
#include "config/save_config.h"
#include "wifi/setup_wifi.h"
#include "client/transfer.h"
#include "client/anchor.h"
#include "client/bonk.h"
#include "client/helius.h"
#include "SolanaSDK/keypair.h"
#include "server/web_server.h"

unsigned long lastCheckTime = 0;
unsigned int buttonHighCount = 0;

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(115200);

  setupWifi();

  rpc();
}

void loop()
{
}