
# ESP32 client for Solana DePIN applications
## Ultimate goal of Skat Energy
At Skat Energy, we believe in electricity network where energy can be traded freely, cheaper and with limited need of government intervention. 
Trading electricity should be as seamless as sending text on Whatsapp or renewing Youtube subsciption.
Moreover, decreasing cost of renewables brings billions of prosumers to the market and we think we have the right solution for them to join the market. 

## On Esp32 client
The main objective is to showcase how to send Solana transactions in C++ from an ESP32 microcontroller. This functionality can be utilized for various Decentralized Physical Infrastructure Networks (DePIN) applications, enabling secure and efficient interactions with the Solana blockchain.

Currently the footprint of Rust Solana SDK in bigger than 20MB which is beyond esp32 16MB constrains. Alternatives are more espensive like Raspberry Pi 4 2GB or Raspberry Zero 2W which limits the speed of worldwide DEPINization :)

This is still a work in progress with following backlog:
- [ ] Add tests to ensure that Solana transactions are called correctly
- [ ] Add a WebSocker listener to account state changes on Solana in order to turn on or off the hardware
- [ ] Show hardware state in frontend
- [ ] Show hardware state on eInk screen
- [ ] Offload frontend to CDN if needed

## Configuration Details

### Platform and Board

- **Platform**: `espressif32`
  - This specifies the development platform for ESP32 microcontrollers.
- **Board**: `nodemcu-32s`
  - This specifies the specific board model being used, which is the NodeMCU-32S.

### Framework

- **Framework**: `arduino`
  - This specifies that the Arduino framework is being used for development.

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