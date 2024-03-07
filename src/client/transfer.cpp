#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <string>
#include <vector>
#include <base64.h>
#include "config/save_config.h"
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

const uint64_t LAMPORTS_PER_SOL = 1000000000;

std::vector<uint8_t> get_transfer_data(uint64_t number) {
    std::vector<uint8_t> amount_hex;
    std::vector<uint8_t> transferData = {
        0x02, 0x00, 0x00, 0x00};
    
    // Extract individual bytes
    amount_hex.push_back((number >> 56) & 0xFF);
    amount_hex.push_back((number >> 48) & 0xFF);
    amount_hex.push_back((number >> 40) & 0xFF);
    amount_hex.push_back((number >> 32) & 0xFF);
    amount_hex.push_back((number >> 24) & 0xFF);
    amount_hex.push_back((number >> 16) & 0xFF);
    amount_hex.push_back((number >> 8) & 0xFF);
    amount_hex.push_back(number & 0xFF);
    std::reverse(amount_hex.begin(), amount_hex.end());
    transferData.insert(transferData.end(), amount_hex.begin(), amount_hex.end());

    return transferData;
}

bool transfer(float amount)
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
    unsigned char configSecretKey[64];

    getPrivateKey(configSecretKey);

    Keypair kp = Keypair(configSecretKey);
    Signer signer = Signer(kp);

    String sender = Base58::trimEncode(signer.publicKey().serialize()).c_str();
    Serial.print("SENDER: ");
    Serial.println(sender);

    PublicKey recipient = PublicKey(Base58::trimDecode("BDEECMrE5dv4cc5na6Fi8sNkfzYxckd6ZjsuEzp7hXnJ"));
    Serial.print("RECIPIENT: ");
    Serial.println(recipient.toBase58().c_str());

    AccountMeta *user = AccountMeta::newWritable(signer.publicKey(), true);
    AccountMeta *receiver = AccountMeta::newWritable(recipient, false);
    std::vector<uint8_t> transferData = get_transfer_data(amount*LAMPORTS_PER_SOL);

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
