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

// 1 = trump
// 2 = biden

String voteIx(int x)
{
  const char *devnetRPC = "https://parental-ardenia-fast-devnet.helius-rpc.com/";
  const char *mainnetRPC = "https://cecily-q1u5dh-fast-mainnet.helius-rpc.com/";

  // Establish WiFi connection (code from previous snippet)

  // Make the REST API call
  if (WiFi.status() == WL_CONNECTED)
  {
    // http get request for ix data from frontend

    Connection connection = Connection(devnetRPC, Commitment::confirmed);

    BlockhashWithExpiryBlockHeight recentBlockhash = connection.getLatestBlockhash();

    // Get secret key from saved config
    String configSecretKey;

    getPrivateKey(configSecretKey);

    Keypair kp = stringToKeypair(configSecretKey.c_str());
    Signer signer = Signer(kp);

    String payer = Base58::trimEncode(signer.publicKey().serialize()).c_str();
    Serial.print("PAYER: ");
    Serial.println(payer);

    PublicKey voteAccount = PublicKey(Base58::trimDecode("C4e9fghY5Wf9Yi4V5w59jQMk1SQmG9Yrfx7xCB985ztg"));

    AccountMeta *payerAccountMeta = AccountMeta::newWritable(signer.publicKey(), true);
    AccountMeta *voteAccountMeta = AccountMeta::newWritable(voteAccount, false);

    PublicKey systemProgramId = SystemProgram::id();
    AccountMeta *systemProgramAccountMeta = AccountMeta::newReadonly(systemProgramId, false);

    std::vector<uint8_t> transferData;
    std::vector<AccountMeta> accounts;

    std::vector<uint8_t> voteTrumpData = bufferToHexUint8Vector({55, 186, 159, 175, 72, 116, 84, 79});
    std::vector<uint8_t> voteBidenData = bufferToHexUint8Vector({152, 120, 185, 205, 231, 78, 197, 224});

    std::vector<AccountMeta> init_accounts = {*payerAccountMeta, *voteAccountMeta, *systemProgramAccountMeta};
    std::vector<AccountMeta> vote_accounts = {*payerAccountMeta, *voteAccountMeta};

    if (x == 1)
    {
      transferData = voteTrumpData;
      accounts = vote_accounts;
    }
    else if (x == 2)
    {
      transferData = voteBidenData;
      accounts = vote_accounts;
    }

    Serial.println("HEX DATA: ");
    for (auto byte : transferData)
    {
      Serial.print(byte, HEX);
      Serial.print(", ");
    }
    Serial.println(" ");

    PublicKey voteProgramId = PublicKey(Base58::trimDecode("8tLuLj11sZgqgfCrzpHAhS4QN8RZh8uknqmAyLKKMVow"));

    std::vector<uint8_t> accountBytes;
    for (auto &account : accounts)
    {
      auto serializedAccount = account.serialize();
      accountBytes.insert(accountBytes.end(), serializedAccount.begin(), serializedAccount.end());
    }

    Instruction ix = Instruction::newWithBytes(
        voteProgramId, transferData,
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

    Signers signers(signerVector);

    tx.sign(signers, recentBlockhash.blockhash);

    Signature returnedSignature = connection.sendTransaction(tx);

    Serial.print("SIGNATURE: ");
    Serial.println(returnedSignature.toString().c_str());

    return returnedSignature.toString().c_str();
  }

  return "";
}
