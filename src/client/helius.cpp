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

String rpc()
{
  const char *devnetRPC = "https://parental-ardenia-fast-devnet.helius-rpc.com/";
  const char *mainnetRPC = "https://cecily-q1u5dh-fast-mainnet.helius-rpc.com/";

  if (WiFi.status() == WL_CONNECTED)
  {
    Connection connection = Connection(devnetRPC, Commitment::confirmed);

    BlockhashWithExpiryBlockHeight recentBlockhash = connection.getLatestBlockhash();

    // Get secret key from saved config
    String configSecretKey;

    getPrivateKey(configSecretKey);

    Keypair kp = Keypair(stringToKeypair(configSecretKey.c_str()));
    Signer signer = Signer(kp);

    String payer = Base58::trimEncode(signer.publicKey().serialize()).c_str();
    Serial.print("PAYER: ");
    Serial.println(payer);

    Keypair data = Keypair::generate();
    Serial.print("DATA: ");
    Serial.println(data.publicKey.toBase58().c_str());

    AccountMeta *payerAccountMeta = AccountMeta::newWritable(signer.publicKey(), true);
    AccountMeta *dataAccountMeta = AccountMeta::newWritable(data.publicKey, true);
    PublicKey systemProgramId = SystemProgram::id();
    AccountMeta *systemProgramAccountMeta = AccountMeta::newReadonly(systemProgramId, false);

    std::vector<uint8_t> transferData = {0xaf, 0xaf, 0x6d, 0x1f, 0x0d, 0x98, 0x9b, 0xed, 0x12, 0x00, 0x00, 0x00, 0x57, 0x68, 0x61, 0x74, 0xe2, 0x80, 0x99, 0x73, 0x20, 0x79, 0x6f, 0x75, 0x72, 0x20, 0x52, 0x50, 0x43, 0x3f};

    std::vector<AccountMeta> accounts = {*dataAccountMeta, *payerAccountMeta, *systemProgramAccountMeta};

    PublicKey axsProgramId = PublicKey(Base58::trimDecode("UvYJbGcunVqiZT6v4pZXXd6CRSa4XnhsNPGUcyVYW1V"));

    std::vector<uint8_t> accountBytes;
    for (auto &account : accounts)
    {
      auto serializedAccount = account.serialize();
      accountBytes.insert(accountBytes.end(), serializedAccount.begin(), serializedAccount.end());
    }

    Instruction ix = Instruction::newWithBytes(
        axsProgramId, transferData,
        accounts);

    std::vector<uint8_t> transferIx = ix.serialize();

    std::vector<Instruction> ixs;
    ixs.push_back(ix);

    std::optional<PublicKey> feePayer;

    feePayer = signer.publicKey();

    Message msg = Message::newWithBlockhash(ixs, feePayer, recentBlockhash.blockhash);

    std::vector<uint8_t> serializedMsg = msg.serialize();

    Transaction tx = Transaction(msg);

    std::vector<Signer> signerVector;
    signerVector.push_back(signer);
    signerVector.push_back(data);

    Signers signers(signerVector);

    tx.sign(signers, recentBlockhash.blockhash);

    std::vector<uint8_t> serializedTx = tx.serialize();

    Signature returnedSignature = connection.sendTransaction(tx);

    Serial.print("SIGNATURE: ");
    Serial.println(returnedSignature.toString().c_str());
  } 
  return "";
}
