#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <string>
#include <vector>
#include <base64.h>
#include "config/save_config.h"
#include "solana/keypair.h"
#include "solana/signer.h"
#include <solana/base58.h>
#include <solana/message.h>
#include <solana/instruction.h>
#include <solana/account_meta.h>
#include <solana/programs/system_program.h>

bool transfer()
{
  const char *devnetRPC = "https://kora-disz8d-fast-devnet.helius-rpc.com/";
  const char *mainnetRPC = "https://cecily-q1u5dh-fast-mainnet.helius-rpc.com/";

  // Establish WiFi connection (code from previous snippet)

  // Make the REST API call
  if (WiFi.status() == WL_CONNECTED)
  {
    HTTPClient http;
    // http.begin(mainnetRPC);
    http.begin(devnetRPC);
    int get_latest_blockhash_req = http.POST("{\"id\":34,\"jsonrpc\":\"2.0\",\"method\":\"getLatestBlockhash\",\"params\":[{\"commitment\":\"processed\"}]}");

    if (get_latest_blockhash_req > 0)
    {
      String payload = http.getString();
      // Serial.println(payload);  // Print raw response

      // Deserialize JSON data
      DynamicJsonDocument doc(1024); // Adjust capacity as needed
      deserializeJson(doc, payload);

      // Access parsed data
      String blockhash = doc["result"]["value"]["blockhash"];

      Serial.println("blockhash: " + blockhash);
      Serial.println("");

      unsigned char config_secret_key[64];

      get_private_key(config_secret_key);

      Keypair kp = Keypair(config_secret_key);
      Signer signer = Signer(kp);

      Serial.println("");
      Serial.print("Public Key: ");
      Serial.println(kp.publicKey.toBase58().c_str());

      String sender = String(kp.publicKey.toBase58().c_str());

      String message = "{\"header\":{\"numRequiredSignatures\":1,\"numReadonlySignedAccounts\":0,\"numReadonlyUnsignedAccounts\":1},\"staticAccountKeys\":[\"" + sender + "\",\"11111111111111111111111111111111\"],\"recentBlockhash\":\"" + blockhash + "\",\"compiledInstructions\":[{\"programIdIndex\":1,\"accountKeyIndexes\":[0,0],\"data\":{\"type\":\"Buffer\",\"data\":[2,0,0,0,136,19,0,0,0,0,0,0]}}],\"addressTableLookups\":[]}";

      AccountMeta *user = AccountMeta::newWritable(signer.publicKey(), true);
      Serial.println("1");
      std::vector<uint8_t> transferData = {
          0x02, 0x00, 0x00, 0x00, 0x80, 0xf0, 0xfa, 0x02, 0x00, 0x00, 0x00, 0x00};
      Serial.println("2");

      std::vector<AccountMeta> accounts = {*user};
      Serial.println("ACCOUNT SIZE:");
      Serial.println(accounts.size());
      std::vector<uint8_t> accountBytes;
      Serial.println("4");
      for (auto &account : accounts)
      {
        Serial.println("LOOP");
        Serial.println(account.isSigner);
        auto serializedAccount = account.serialize();
        accountBytes.insert(accountBytes.end(), serializedAccount.begin(), serializedAccount.end());
      }
      Serial.println("Account BYTES:");
      Serial.println(accountBytes.size());

      Instruction ix = Instruction::newWithBytes(
          SystemProgram::id(), transferData,
          accounts);
      Serial.println("6");

      std::vector<uint8_t> vec = ix.serialize();
      Serial.println("7");
      String str;
      for (uint8_t byte : vec)
      {
        str += String(byte, HEX);
      }
      Serial.println("IX: " + str);

      // Message msg = Message()

      String signature = String(signer.sign(std::string(message.c_str())).c_str());
      Serial.println("");
      Serial.print("Signature: ");
      Serial.println(signature.c_str());

      String send_transaction_data = "{\"signatures\":[\"" + signature + "\"],\"message\": " + message + "}";

      Serial.println("");
      Serial.print("Data: ");
      Serial.println(send_transaction_data);

      String send_transaction_param = String(base58Encode(reinterpret_cast<const unsigned char *>(send_transaction_data.c_str()), send_transaction_data.length()).c_str());

      Serial.println("");
      Serial.print("Signed Transaction: ");
      Serial.println(send_transaction_param);

      String req_data = "{\"jsonrpc\":\"2.0\",\"id\":1,\"method\":\"sendTransaction\",\"params\":[\"" + send_transaction_param + "\"]}";

      Serial.println("");
      Serial.print("Transaction(base64): ");
      Serial.println(base64::encode(send_transaction_data));

      Serial.println("");
      Serial.print("Sending Transaction: ");
      Serial.println(req_data);

      int send_transaction_req = http.POST(req_data);

      if (send_transaction_req > 0)
      {
        String send_transaction_payload = http.getString();
        Serial.println("");
        Serial.println(send_transaction_payload);
        DynamicJsonDocument send_transaction_doc(1024);
        deserializeJson(send_transaction_doc, send_transaction_payload);

        // Access parsed data
        String signature = send_transaction_doc["result"];

        Serial.println("");
        Serial.print("Sent Signature: ");
        Serial.println(signature.c_str());
      }
      else
      {
        Serial.print("HTTP error: ");
        Serial.println(send_transaction_req);
        return false;
      }
    }
    else
    {
      Serial.print("HTTP error: ");
      Serial.println(get_latest_blockhash_req);
      return false;
    }

    http.end();
  }

  return true;
}