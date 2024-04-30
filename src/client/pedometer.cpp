#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_ADXL345_U.h>
#include <WiFi.h>
#include <WebServer.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "client/bonk.h"


#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1

Adafruit_ADXL345_Unified accel = Adafruit_ADXL345_Unified(12345);
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

const float threshold = 1.2; // Adjust this threshold for step detection sensitivity
const int bufferLength = 15; // Number of accelerometer readings in the buffer
float buffer[bufferLength];
int bufferIndex = 0;
int stepCount = 0;
bool stepDetected = false;

const unsigned long debounceDelay = 300; // Debounce delay in milliseconds
unsigned long lastStepTime = 0;

void updateOledDisplay() {
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.print("Step Count: ");
  display.print(stepCount);
  display.display();
}

void adxlsetup() {
  Wire.begin();

  if (!accel.begin()) {
    Serial.println("Could not find a valid ADXL345 sensor, check wiring!");
    while (1);
  }

  accel.setRange(ADXL345_RANGE_16_G);

  delay(1000);
}

void oledsetup() {
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.display();
  delay(2000);
  display.clearDisplay();
  delay(1000);
}

void step_count() {
  sensors_event_t event;
  accel.getEvent(&event);

  float accelerationX = event.acceleration.x;
  float accelerationY = event.acceleration.y;
  float accelerationZ = event.acceleration.z;

  float accelerationMagnitude = sqrt(accelerationX * accelerationX +
                                     accelerationY * accelerationY +
                                     accelerationZ * accelerationZ);

  buffer[bufferIndex] = accelerationMagnitude;
  bufferIndex = (bufferIndex + 1) % bufferLength;

  // Detect a step if the current magnitude is greater than the average of the buffer by the threshold
  float avgMagnitude = 0;
  for (int i = 0; i < bufferLength; i++) {
    avgMagnitude += buffer[i];
  }
  avgMagnitude /= bufferLength;

  unsigned long currentMillis = millis();

  if (accelerationMagnitude > (avgMagnitude + threshold)) {
    if (!stepDetected && (currentMillis - lastStepTime) > debounceDelay) {
      stepCount++;
      stepDetected = true;
      lastStepTime = currentMillis;

      

      // Update OLED display with step count
      updateOledDisplay();

      if(stepCount == 100) {
        Serial.println("Step detected!");
        Serial.print("Step count: ");
        Serial.println(stepCount);
        transferBonk();
        stepCount = 0;
      }
    }
  } else {
    stepDetected = false;
  }

  
}