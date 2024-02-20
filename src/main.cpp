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

  // const char *blockhash = "5cVwZX6dCwk3ZXvuFNS6L8APZpXJTrjJgSDbnkaKUtrs";
  // std::vector<uint8_t> blockhashVec = Base58::decode(blockhash);
  // Serial.print("PRE-HASH:");
  // Base58::printArray(blockhashVec);

  // std::array<uint8_t, 32> blockhashArr = {0};
  // std::memcpy(blockhashArr.data(), &blockhashVec, blockhashVec.size());

  // Hash recentBlockhash = Hash(blockhashArr);

  // Serial.print("HASH:");
  // Serial.println("");
  // for (auto a : recentBlockhash.data)
  // {
  //   Serial.print(a);
  //   Serial.print(",");
  // }
  // Serial.println("");

  // std::vector<uint8_t> arr(blockhashVec.begin(), blockhashVec.end());
  // std::string bsStr = Base58::encode(arr);
  // Serial.println(String(bsStr.c_str()));
}

void loop()
{
  // put your main code here, to run repeatedly:
}
