#include <Arduino.h>
#include "config/save_config.h"
#include "wifi/setup_wifi.h"
#include "client/transfer.h"
#include "client/anchor.h"
#include "SolanaSDK/keypair.h"
#include "server/web_server.h"

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(115200);

  setupWifi();

  // transfer();

  // anchor();
}
