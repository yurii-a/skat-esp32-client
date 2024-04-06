#include <Arduino.h>
#include "config/save_config.h"
#include "wifi/setup_wifi.h"
#include "client/transfer.h"
#include "client/anchor_ix.h"
#include "client/vote_ix.h"
#include "client/stake.h"
#include "SolanaSDK/keypair.h"
#include "server/web_server.h"
#include<string.h>
#include <sodium/crypto_hash_sha256.h>
#include <ArduinoJson.h>
#include <fstream>
#include <nlohmann/json.hpp>
#include <iostream>
// using json = nlohmann::json;

void setup(){
  // put your setup code here, to run once:
  Serial.begin(115200);

  // // saveConfig();

  setupWifi();

  vote_ix(1);

  // stake(1); // deposit
  // // rpc();

  // transfer(0.03);

  // anchor_ix();


  // std::ifstream f("idl.json");
  // // json data = json::parse(f);
  // DynamicJsonDocument doc(1024); // Adjust capacity as needed
  // deserializeJson(doc, payload);
  // Serial.println(data["version"]);
  

    // const std::string& SIGHASH_GLOBAL_NAMESPACE = "global";

    // std::vector<uint8_t> transferData = {0xaf, 0xaf, 0x6d, 0x1f, 0x0d, 0x98, 0x9b, 0xed, 0x09, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    // for(const auto& byte : transferData) {
    //     Serial.print(byte, HEX);
    // }
    // Serial.println("");

    // std::string snakeCaseIxName = "";
    // std::string ixName = "initialize";

    // char c = tolower(ixName[0]); 
    // snakeCaseIxName+=(char(c)); 
    // for (int i = 1; i < ixName.length(); i++) { 

    //     char ch = ixName[i]; 
    //     if (isupper(ch)) { 
    //         snakeCaseIxName = snakeCaseIxName + '_'; 
    //         snakeCaseIxName+=char(tolower(ch)); 
    //     } 
    //     else { 
    //         snakeCaseIxName = snakeCaseIxName + ch; 
    //     } 
    // }
    // std::string preimage = SIGHASH_GLOBAL_NAMESPACE+":"+snakeCaseIxName;




  
}
