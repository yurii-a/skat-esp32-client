#include <Arduino.h>
#include <string>

void sol(uint64_t num, int* outputInts) {
    String hexString = String(num, HEX);
    Serial.println(hexString);
    char* hexCharArr = &hexString[0];
    int intCount = 0;

    for (uint8_t i = 0; i < hexString.length(); i += 2) {
        char byteChars[3] = { hexCharArr[i], hexCharArr[i + 1], 0 };
        outputInts[intCount] = strtol(byteChars, NULL, 16);
        Serial.println(outputInts[intCount]);
        intCount++;
    }
}