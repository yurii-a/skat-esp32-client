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
      const char *blockhash = doc["result"]["value"]["blockhash"];

      Serial.print("blockhash: ");
      Serial.println(String(blockhash));
      Serial.println("");

      unsigned char config_secret_key[64];

      get_private_key(config_secret_key);

      Keypair kp = Keypair(config_secret_key);
      Signer signer = Signer(kp);

      Serial.println("");
      Serial.print("Public Key: ");
      Serial.println(kp.publicKey.toBase58().c_str());

      String sender = Base58::trimEncode(signer.publicKey().serialize()).c_str();

      String message = "{\"header\":{\"numRequiredSignatures\":1,\"numReadonlySignedAccounts\":0,\"numReadonlyUnsignedAccounts\":1},\"staticAccountKeys\":[\"" + sender + "\",\"11111111111111111111111111111111\"],\"recentBlockhash\":\"" + blockhash + "\",\"compiledInstructions\":[{\"programIdIndex\":1,\"accountKeyIndexes\":[0,0],\"data\":{\"type\":\"Buffer\",\"data\":[2, 0, 0, 0, 128, 240, 250, 2, 0, 0, 0, 0]}}],\"addressTableLookups\":[]}";

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

      std::vector<uint8_t> transferIx = ix.serialize();
      for (auto byte : transferIx)
      {
        Serial.print(byte);
        Serial.print(", ");
      }
      Serial.println("");

      std::vector<Instruction> ixs;
      ixs.push_back(ix);

      std::optional<PublicKey> payer;

      payer = signer.publicKey();

      std::vector<uint8_t> recentBlockhash = Base58::trimDecode(blockhash);

      Base58::printArray(recentBlockhash);

      Message msg = Message::newWithBlockhash(ixs, payer, recentBlockhash);

      Transaction tx = Transaction(msg);

      std::vector<Signer> signerVector;
      signerVector.push_back(signer);

      Signers signers(signerVector);

      tx.sign(signers, Hash(recentBlockhash));

      Signature signature = tx.signatures[0];
      std::vector<uint8_t> signatureBytes = signature.serialize();

      std::vector<uint8_t> serializedMessage = tx.messageData();

      std::string bs64Tx(serializedMessage.begin(), serializedMessage.end());

      Serial.print("SERIALIZED MSG: ");
      String serializedMsg = Base58::trimEncode(msg.serialize()).c_str();
      Serial.println(serializedMsg);
      Serial.println();

      Serial.print("SERIALIZED TX: ");
      String serializedTx = Base58::trimEncode(tx.serialize()).c_str();
      Serial.println(serializedTx);
      Serial.println();

      Serial.print("DESERIALIZED TX: ");
      std::vector<uint8_t> deserializedTx = Base58::trimDecode(serializedTx.c_str());
      for (auto byte : deserializedTx)
      {
        Serial.print(byte);
        Serial.print(", ");
      }
      Serial.println();

      Serial.print("SIGNATURE: ");
      for (auto byte : signatureBytes)
      {
        Serial.print(byte);
        Serial.print(", ");
      }
      Serial.println();

      std::string firstSignature = Base58::trimEncode(signatureBytes);
      Serial.println(firstSignature.c_str());
      String stringSignature(firstSignature.c_str());

      // Convert signatureBytes to a string of comma-separated bytes
      std::stringstream sigBytes;
      sigBytes << '[';
      for (size_t i = 0; i < signatureBytes.size(); ++i)
      {
        sigBytes << static_cast<int>(signatureBytes[i]);
        if (i != signatureBytes.size() - 1)
        {
          sigBytes << ",";
        }
      }
      sigBytes << ']';

      String signatureInBytes = sigBytes.str().c_str();

      String sendTransactionData = "{\"signatures\":" + signatureInBytes + ",\"message\":\"" + message + "\"}";

      Serial.println("\nData: " + sendTransactionData);

      std::vector<uint8_t> sendTransactionDataVec(sendTransactionData.begin(), sendTransactionData.end());
      std::string sendTransactionParam = Base58::trimEncode(sendTransactionDataVec);

      Serial.println("\nSigned Transaction: " + String(sendTransactionParam.c_str()));
      std::string ss(sendTransactionParam.begin(), sendTransactionParam.end());
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