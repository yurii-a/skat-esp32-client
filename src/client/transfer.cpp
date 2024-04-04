#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <string>
#include <vector>
#include <base64.h>
#include "config/save_config.h"
#include "config/utils.h"
#include "SolanaSDK/keypair.h"
#include "SolanaSDK/signer.h"
#include <SolanaSDK/base58.h>
#include <SolanaSDK/message.h>
#include <SolanaSDK/instruction.h>
#include <SolanaSDK/account_meta.h>
#include <SolanaSDK/programs/system_program.h>
#include <SolanaSDK/transaction.h>
#include <SolanaSDK/connection.h>

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

    Connection connection = Connection(devnetRPC, Commitment::confirmed);

    BlockhashWithExpiryBlockHeight recentBlockhash = connection.getLatestBlockhash();

    // Get secret key from saved config
    String configSecretKey;

    getPrivateKey(configSecretKey);

    Keypair kp = Keypair(stringToKeypair(configSecretKey.c_str()));
    Signer signer = Signer(kp);

    String sender = Base58::trimEncode(signer.publicKey().serialize()).c_str();
    Serial.print("SENDER: ");
    Serial.println(sender);

    PublicKey recipient = PublicKey(Base58::trimDecode("9qY5qdJ4TNeEcGHTa2FzewjhE9cFj1mAEz9LG9c8sQKy"));
    Serial.print("RECIPIENT: ");
    Serial.println(recipient.toBase58().c_str());

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

    Message msg = Message::newWithBlockhash(ixs, payer, recentBlockhash.blockhash);

    Transaction tx = Transaction(msg);

    std::vector<Signer> signerVector;
    signerVector.push_back(signer);

    Signers signers(signerVector);

    tx.sign(signers, recentBlockhash.blockhash);

    Signature returnedSignature = connection.sendTransaction(tx);

    Serial.print("SIGNATURE: ");
    Serial.println(returnedSignature.toString().c_str());
    separator();

    http.end();
  }

  return true;
}
