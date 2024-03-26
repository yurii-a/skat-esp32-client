#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Wire.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

#define OLED_RESET -1 // Reset pin # (or -1 if sharing Arduino reset pin)

#define BUTTON_PIN 2 // Define your button pin

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void setup() {
  Serial.begin(115200);

  // Initialize display
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;)
      ;
  }

  // Clear the buffer
  display.clearDisplay();
}

void loop() {
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor((SCREEN_WIDTH - (strlen("Project AXS") * 6)) / 2, SCREEN_HEIGHT / 2 - 8); // Center horizontally, slightly above center vertically
    display.println(F("Project AXS"));
    display.display();

  if (digitalRead(BUTTON_PIN) == HIGH) { // Check if button is pressed
    // First display
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor((SCREEN_WIDTH - (strlen("Sending transaction!") * 6)) / 2, SCREEN_HEIGHT / 2 - 8); // Center horizontally, slightly above center vertically
    display.println(F("Sending transaction!"));
    display.display();
    delay(2000);

    // Second display
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor((SCREEN_WIDTH - (strlen("Transaction Sent!") * 6)) / 2, SCREEN_HEIGHT / 2 - 8); // Center horizontally, slightly above center vertically
    display.println(F("Transaction Sent!"));
    display.display();
    delay(2000);

    // Third display
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor((SCREEN_WIDTH - (strlen("Guess what MERT said?") * 6)) / 2, SCREEN_HEIGHT / 2 - 8); // Center horizontally, slightly above center vertically
    display.println(F("Guess what MERT said?"));
    display.display();
    delay(2000);
  }
}
