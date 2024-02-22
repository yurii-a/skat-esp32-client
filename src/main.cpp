#include <Arduino.h>
#include "config/save_config.h"
#include "wifi/setup_wifi.h"
#include "solana/helpers.h"
#include "client/transfer.h"
#include "solana/hash.h"
#include "solana/base58.h"

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(115200);

  // rpc();

  // save_config();

  setup_wifi();

  // test_sign();

  transfer();
}

void loop()
{
  // put your main code here, to run repeatedly:
}
