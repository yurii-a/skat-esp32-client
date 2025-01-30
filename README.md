# Project Configuration for NodeMCU-32S

This project is configured to use PlatformIO for development with the NodeMCU-32S board, which is based on the ESP32 microcontroller. The `platformio.ini` file contains various settings and dependencies required for building and uploading the firmware.

## Configuration Details

### Platform and Board

- **Platform**: `espressif32`
  - This specifies the development platform for ESP32 microcontrollers.
- **Board**: `nodemcu-32s`
  - This specifies the specific board model being used, which is the NodeMCU-32S.

### Framework

- **Framework**: `arduino`
  - This specifies that the Arduino framework is being used for development.

### Build Flags

- **build_unflags**: `-std=gnu++11`
  - This unflags the GNU++11 standard.
- **build_flags**: `-std=gnu++17`
  - This sets the C++ standard to GNU++17.
- **debug_build_flags**: `-Os`
  - This sets the optimization level for debugging to `-Os` (optimize for size).

### Build Type

- **build_type**: `debug`
  - This sets the build type to debug, which includes debugging information in the compiled firmware.

### Monitor Filters

- **monitor_filters**: `esp32_exception_decoder`
  - This enables the ESP32 exception decoder filter for the serial monitor, which helps in decoding exceptions and stack traces.

### Library Dependencies

The following libraries are specified as dependencies for this project:

- **bblanchon/ArduinoJson@^7.0.0**
  - A popular JSON library for embedded systems, providing efficient and easy-to-use JSON serialization and deserialization.
- **intrbiz/Crypto@^1.0.0**
  - A library providing cryptographic algorithms and functions, such as hashing, encryption, and decryption.
- **git@github.com:yurii-a/SolanaSDK.git**
  - A custom library hosted on GitHub, providing tools and functions to interact with the Solana blockchain.
- **adafruit/Adafruit SSD1306@^2.5.9**
  - A library for interfacing with the SSD1306 OLED display.
- **adafruit/Adafruit GFX Library@^1.11.9**
  - A core graphics library providing basic drawing routines for various displays.
- **ricmoo/QRCode@^0.0.1**
  - A library for generating QR codes.
- **adafruit/Adafruit Unified Sensor@^1.1.14**
  - A library providing a common interface and abstraction layer for various sensors.
- **adafruit/Adafruit ADXL345@^1.3.4**
  - A library for interfacing with the ADXL345 accelerometer.
- **sparkfun/SparkFun ADXL345 Arduino Library@^1.0.0**
  - Another library for interfacing with the ADXL345 accelerometer, developed by SparkFun.

## Usage

To use this configuration, ensure you have PlatformIO installed and open the project in your preferred IDE (e.g., Visual Studio Code). PlatformIO will automatically use the settings and dependencies specified in the `platformio.ini` file to build and upload the firmware to the NodeMCU-32S board.

For more information on PlatformIO and its configuration options, please visit the [PlatformIO documentation](https://docs.platformio.org/page/projectconf.html).