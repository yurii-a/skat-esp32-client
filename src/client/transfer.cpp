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

void separator()
{
  Serial.println();
}

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

      separator();
      Serial.print("SERIALIZED BLOCKHASH: ");
      Serial.println(String(blockhash));
      separator();

      std::vector<uint8_t> recentBlockhash = Base58::trimDecode(blockhash);

      separator();
      Serial.print("DESERIALIZED BLOCKHASH: ");
      Base58::printArray(recentBlockhash);
      separator();

      std::string reSerializeBlockhash = Base58::trimEncode(recentBlockhash);

      separator();
      Serial.print("RE-SERIALIZED BLOCKHASH: ");
      Serial.print(reSerializeBlockhash.c_str());
      separator();

      unsigned char config_secret_key[64];

      get_private_key(config_secret_key);

      Keypair kp = Keypair(config_secret_key);
      Signer signer = Signer(kp);

      separator();
      Serial.print("SERIALIZED PUBLIC KEY: ");
      std::string serializedPublicKey = kp.publicKey.toBase58();
      Serial.println(serializedPublicKey.c_str());
      separator();

      separator();
      Serial.print("DESERIALIZED PUBLIC KEY: ");
      kp.publicKey.print();
      separator();

      // TODO: Test this
      // std::string reSerializePublicKey = Base58::trimEncode(PublicKey::fromString(serializedPublicKey).value().serialize());

      // separator();
      // Serial.print("RE-SERIALIZED PUBLIC KEY: ");
      // Serial.print(reSerializePublicKey.c_str());
      // separator();

      String sender = Base58::trimEncode(signer.publicKey().serialize()).c_str();

      String message = "{\"header\":{\"numRequiredSignatures\":1,\"numReadonlySignedAccounts\":0,\"numReadonlyUnsignedAccounts\":1},\"staticAccountKeys\":[\"" + sender + "\",\"11111111111111111111111111111111\"],\"recentBlockhash\":\"" + blockhash + "\",\"compiledInstructions\":[{\"programIdIndex\":1,\"accountKeyIndexes\":[0,0],\"data\":{\"type\":\"Buffer\",\"data\":[2,0,0,0,128,240,250,2,0,0,0,0]}}],\"addressTableLookups\":[]}";

      PublicKey recipient = PublicKey(Base58::trimDecode("9qY5qdJ4TNeEcGHTa2FzewjhE9cFj1mAEz9LG9c8sQKy"));

      separator();
      recipient.print();
      separator();

      AccountMeta *user = AccountMeta::newWritable(signer.publicKey(), true);
      AccountMeta *receiver = AccountMeta::newWritable(recipient, false);
      std::vector<uint8_t> transferData = {
          0x02, 0x00, 0x00, 0x00, 0x80, 0xf0, 0xfa, 0x02, 0x00, 0x00, 0x00, 0x00};

      std::vector<AccountMeta> accounts = {*user, *receiver};
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

      std::vector<Instruction> ixs;
      ixs.push_back(ix);

      std::optional<PublicKey> payer;

      payer = signer.publicKey();

      Message msg = Message::newWithBlockhash(ixs, payer, recentBlockhash);

      separator();
      Serial.print("SERIALIZED MSG: ");
      String serializedMsg = Base58::trimEncode(msg.serialize()).c_str();
      Serial.println(serializedMsg);
      separator();

      separator();
      Serial.print("DESERIALIZED MSG: ");
      std::vector<uint8_t> deserializedMsg = Base58::trimDecode(serializedMsg.c_str());
      for (auto byte : deserializedMsg)
      {
        Serial.print(byte);
        Serial.print(",");
      }
      separator();

      std::string reSerializeMsg = Base58::trimEncode(deserializedMsg);

      separator();
      Serial.print("RE-SERIALIZED MSG: ");
      Serial.print(reSerializeMsg.c_str());
      separator();

      Transaction tx = Transaction(msg);

      std::vector<Signer> signerVector;
      signerVector.push_back(signer);

      Signers signers(signerVector);

      tx.sign(signers, Hash(recentBlockhash));

      Signature signature = tx.signatures[0];
      std::vector<uint8_t> serializedSignature = signature.serialize();

      separator();
      Serial.print("SERIALIZED SIGNATURE: ");
      std::string firstSignature = Base58::trimEncode(serializedSignature);
      Serial.println(firstSignature.c_str());
      String stringSignature(firstSignature.c_str());
      separator();

      separator();
      Serial.print("DESERIALIZED SIGNATURE: ");
      for (auto byte : serializedSignature)
      {
        Serial.print(byte);
        Serial.print(",");
      }
      separator();

      std::string reSerializeSignature = Base58::trimEncode(serializedSignature);

      separator();
      Serial.print("RE-SERIALIZED SIGNATURE: ");
      Serial.print(reSerializeSignature.c_str());
      separator();

      separator();
      Serial.print("SERIALIZED TX: ");
      String serializedTx = Base58::trimEncode(tx.serialize()).c_str();
      Serial.print(serializedTx);
      separator();

      separator();
      Serial.print("DESERIALIZED TX: ");
      std::vector<uint8_t> deserializedTx = Base58::trimDecode(serializedTx.c_str());
      for (auto byte : deserializedTx)
      {
        Serial.print(byte);
        Serial.print(",");
      }
      separator();

      std::string reSerializedTx = Base58::trimEncode(deserializedTx);

      separator();
      Serial.print("RE-SERIALIZED TX: ");
      Serial.print(reSerializedTx.c_str());
      separator();

      separator();
      Serial.print("TX SIZE: ");
      Serial.print(deserializedTx.size());
      separator();

      // String reqDataBase64 = base64::encode(serializedTx.c_str());

      // separator();
      // Serial.print("Transaction(base64): ");
      // Serial.println(reqDataBase64);
      // Serial.println(reqDataBase64.length());
      // separator();

      String configObject = "{\"encoding\":\"base58\", \"skipPreflight\":true, \"preflightCommitment\":\"finalized\", \"maxRetries\":5}";

      String reqData = "{\"jsonrpc\":\"2.0\",\"id\":1,\"method\":\"sendTransaction\",\"params\":[\"" + String(serializedTx.c_str()) + "\", " + configObject + "]}";

      separator();
      Serial.print("Sending Transaction: ");
      Serial.println(reqData);
      separator();

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

        separator();
        Serial.print("Sent Signature: ");
        Serial.println(signature.c_str());
        separator();
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
