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

// 0 = stake
// 1 = unstake

String stakeIx(int x, uint64_t amount)
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

    PublicKey stakeAccount = PublicKey(Base58::trimDecode("Byr8ZpT4LT9mowoN8zePxn5mPgJw9vT6moUZz2UiuYZZ"));

    AccountMeta *payerAccountMeta = AccountMeta::newWritable(signer.publicKey(), true);
    AccountMeta *stakeAccountMeta = AccountMeta::newWritable(stakeAccount, false);

    PublicKey systemProgramId = SystemProgram::id();
    AccountMeta *systemProgramAccountMeta = AccountMeta::newReadonly(systemProgramId, false);

    std::vector<uint8_t> transferData;
    std::vector<AccountMeta> accounts;

    std::vector<uint8_t> stakeData = bufferToHexUint8Vector({206, 176, 202, 18, 200, 209, 179, 108});
    std::vector<uint8_t> unstakeData = bufferToHexUint8Vector({90, 95, 107, 42, 205, 124, 50, 225});

    std::vector<AccountMeta> stakeAccounts = {*stakeAccountMeta, *payerAccountMeta, *systemProgramAccountMeta};

    if (x == 0)
    {
      transferData = stakeData;
      accounts = stakeAccounts;
    }
    else if (x == 1)
    {
      transferData = unstakeData;
      accounts = stakeAccounts;
    }

    // Convert amount to bytes and append to transferData
    std::vector<uint8_t> amountBytes(sizeof(amount));
    memcpy(amountBytes.data(), &amount, sizeof(amount));
    transferData.insert(transferData.end(), amountBytes.begin(), amountBytes.end());

    Serial.println("HEX DATA: ");
    for (auto byte : transferData)
    {
      Serial.print(byte, HEX);
      Serial.print(", ");
    }
    Serial.println(" ");

    PublicKey stakeProgramId = PublicKey(Base58::trimDecode("3wWnXqbxVfpFRr1VgiwJykVZSqwJcqAwMKtoJ9N3YjRj"));

    std::vector<uint8_t> accountBytes;
    for (auto &account : accounts)
    {
      auto serializedAccount = account.serialize();
      accountBytes.insert(accountBytes.end(), serializedAccount.begin(), serializedAccount.end());
    }

    Instruction ix = Instruction::newWithBytes(
        stakeProgramId, transferData,
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
