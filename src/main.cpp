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
#include "client/pedometer.h"
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
// #define SCREEN_WIDTH 128 // OLED display width,  in pixels
// #define SCREEN_HEIGHT 64 // OLED display height, in pixels
// // declare an SSD1306 display object connected to I2C
// Adafruit_SSD1306 oled(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

void setup()
{
  Serial.begin(115200);
  setupWifi();
  oledsetup();
  // adxlsetup();
}

void loop()
{
  axsServer.handleClient();
  // step_count();

}