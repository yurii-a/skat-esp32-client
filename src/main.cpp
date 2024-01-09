#include <Arduino.h>
#include "config/saveConfig.h"
#include "wifi/setupWifi.h"
#include "client/helius.h"
#include "crypto/keypair.h"

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  
  // saveConfig();

  setupWifi();

  getKeypairFromSeed();
}

void loop() {
  // put your main code here, to run repeatedly:
  // call();
  // delay(3000);
}
