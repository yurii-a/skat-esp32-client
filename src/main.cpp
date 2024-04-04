#include <Arduino.h>
#include "config/save_config.h"
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

unsigned long lastCheckTime = 0;
unsigned int buttonHighCount = 0;

#define OLED_WIDTH 128
#define OLED_HEIGHT 64

#define BUTTON_PIN 2

Adafruit_SSD1306 display(OLED_WIDTH, OLED_HEIGHT, &Wire, -1);

void displayQRCode(const char* text) {
  QRCode qrcode;

  uint8_t qrcodeData[qrcode_getBufferSize(3)];
  qrcode_initText(&qrcode, qrcodeData, 3, 0, text);

  int scale = min(OLED_WIDTH / qrcode.size, OLED_HEIGHT / qrcode.size);

  int shiftX = (OLED_WIDTH - qrcode.size * scale) / 2;
  int shiftY = (OLED_HEIGHT - qrcode.size * scale) / 2;

  uint16_t fillColor = WHITE;
  uint16_t clearColor = BLACK;

  for (uint8_t y = 0; y < qrcode.size; y++) {
    for (uint8_t x = 0; x < qrcode.size; x++) {
      if (qrcode_getModule(&qrcode, x, y)) {
        display.fillRect(shiftX + x * scale, shiftY + y * scale, scale, scale, fillColor);
      } else {
        display.fillRect(shiftX + x * scale, shiftY + y * scale, scale, scale, clearColor);
      }
    }
  }
  display.display();
}

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(115200);

  setupWifi();

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
}

void loop()
{
  if (digitalRead(BUTTON_PIN) == HIGH) {

    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor((OLED_WIDTH - (strlen("Project AXS") * 6)) / 2, OLED_HEIGHT / 2 - 8); // Center horizontally, slightly above center vertically
    display.println(F("Project AXS"));
    display.display();

    String sig = rpc();

    Serial.println(sig);

    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor((OLED_WIDTH - (strlen("Transaction sent!") * 6)) / 2, OLED_HEIGHT / 2 - 8); // Center horizontally, slightly above center vertically
    display.println(F("Transaction sent!"));
    display.display();
    
    display.clearDisplay();

    String explorer = "https://explorer.solana.com/tx/" + sig + "?cluster=devnet";

      displayQRCode(explorer.c_str());
  }
}