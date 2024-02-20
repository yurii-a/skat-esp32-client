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
#include <solana/transaction.h>

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
      std::vector<uint8_t> transferData = {
          0x02, 0x00, 0x00, 0x00, 0x80, 0xf0, 0xfa, 0x02, 0x00, 0x00, 0x00, 0x00};

      std::vector<AccountMeta> accounts = {*user};
      std::vector<uint8_t> accountBytes;
      for (auto &account : accounts)
      {
        auto serializedAccount = account.serialize();
        accountBytes.insert(accountBytes.end(), serializedAccount.begin(), serializedAccount.end());
      }

      Instruction ix = Instruction::newWithBytes(
          SystemProgram::id(), transferData,
          accounts);

      std::vector<uint8_t> vec = ix.serialize();
      String str;
      for (uint8_t byte : vec)
      {
        str += String(byte, HEX);
      }
      Serial.println("IX: " + str);

      std::vector<Instruction> ixs;
      ixs.push_back(ix);

      std::optional<PublicKey> payer;

      payer = signer.publicKey();

      Base58::printArray(Base58::decode(blockhash.c_str()));

      std::array<uint8_t, 32> blockhashArr = {0};
      std::memcpy(blockhashArr.data(), blockhash.c_str(), std::strlen(blockhash.c_str()));

      Message msg = Message::newWithBlockhash(ixs, payer, blockhashArr);

      Transaction tx = Transaction(msg);

      std::vector<Signer> signerVector;
      signerVector.push_back(signer);

      Signers signers(signerVector);

      tx.sign(signers, Hash::newFromArray(blockhashArr));

      String signature = tx.signatures[0].toString().c_str();

      std::vector<uint8_t> serializedMessage = tx.messageData();

      std::string bs64Tx(serializedMessage.begin(), serializedMessage.end());

      Serial.print("SERIALIZED TX: ");
      Serial.println(bs64Tx.c_str());
      Serial.println();

      Serial.print("SIGNATURE: ");
      Serial.println(signature);
      Serial.println();

      String sendTransactionData = "{\"signatures\":[\"" + signature + "\"],\"message\": " + message + "}";
      Serial.println("\nData: " + sendTransactionData);

      std::vector<uint8_t> sendTransactionDataVec(sendTransactionData.begin(), sendTransactionData.end());
      std::string sendTransactionParam = Base58::trimEncode(sendTransactionDataVec);

      Serial.println("\nSigned Transaction: " + String(sendTransactionParam.c_str()));
      std::string ss(serializedMessage.begin(), serializedMessage.end());
      String reqDataBase64 = base64::encode(ss.c_str());
      Serial.println("\nTransaction(base64): " + reqDataBase64);

      String reqData = "{\"jsonrpc\":\"2.0\",\"id\":1,\"method\":\"sendTransaction\",\"params\":[\"" + String(sendTransactionParam.c_str()) + "\"]}";

      Serial.println("\nSending Transaction: " + reqData);

      int sendTransactionReq = http.POST(reqData);

      if (sendTransactionReq > 0)
      {
        String sendTransactionPayload = http.getString();
        Serial.println("");
        Serial.println(sendTransactionPayload);
        DynamicJsonDocument sendTransactionDoc(1024);
        deserializeJson(sendTransactionDoc, sendTransactionPayload);

        // Access parsed data
        String signature = sendTransactionDoc["result"];

        Serial.println("");
        Serial.print("Sent Signature: ");
        Serial.println(signature.c_str());
      }
      else
      {
        Serial.print("HTTP error: ");
        Serial.println(sendTransactionReq);
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