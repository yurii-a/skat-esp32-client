#include <Arduino.h>
#include "config/save_config.h"
#include "wifi/setup_wifi.h"
#include "client/transfer.h"
#include "client/anchor.h"
#include "client/bonk.h"
#include "SolanaSDK/keypair.h"
#include "server/web_server.h"

#define LED_PIN 15
#define BUTTON_PIN 2
#define CHECK_INTERVAL 1000 // Interval for checking button state (milliseconds)
#define COUNT_THRESHOLD 3   // Number of times button pin needs to be high
#define GRACE_TIME 15000
#define RESET_TIME 3500 // Time in milliseconds to reset the counter

unsigned long lastCheckTime = 0;
unsigned int buttonHighCount = 0;

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(115200);

  setupWifi();

  pinMode(LED_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT);
}

void loop()
{
  axsServer.handleClient();
  unsigned long currentTime = millis();

  if (currentTime - lastCheckTime >= CHECK_INTERVAL)
  {
    if (digitalRead(BUTTON_PIN) == HIGH)
    {
      lastCheckTime = currentTime;
      buttonHighCount++;
      Serial.println("HONK");
    }
    else
    {
      digitalWrite(LED_PIN, LOW);
    }
  }

  if (buttonHighCount >= COUNT_THRESHOLD && currentTime - lastCheckTime < GRACE_TIME)
  {
    buttonHighCount = 0;
    lastCheckTime = currentTime;
    digitalWrite(LED_PIN, HIGH);
    transferBonk();
  }

  if (currentTime - lastCheckTime >= RESET_TIME)
  {
    buttonHighCount = 0;
  }
}