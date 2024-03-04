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

bool anchor()
{
  const char *devnetRPC = "https://kora-disz8d-fast-devnet.helius-rpc.com/";
  const char *mainnetRPC = "https://cecily-q1u5dh-fast-mainnet.helius-rpc.com/";

  // Establish WiFi connection (code from previous snippet)

  // Make the REST API call
  if (WiFi.status() == WL_CONNECTED)
  {

    Connection connection = Connection(devnetRPC, Commitment::confirmed);

    BlockhashWithExpiryBlockHeight recentBlockhash = connection.getLatestBlockhash();

    // Get secret key from saved config
    unsigned char configSecretKey[64];

    getPrivateKey(configSecretKey);

    Keypair kp = Keypair(configSecretKey);
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

    std::vector<uint8_t> transferData = {0xaf, 0xaf, 0x6d, 0x1f, 0x0d, 0x98, 0x9b, 0xed, 0x09, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

    std::vector<AccountMeta> accounts = {*dataAccountMeta, *payerAccountMeta, *systemProgramAccountMeta};

    PublicKey axsProgramId = PublicKey(Base58::trimDecode("38m8XgmGy4p1nstZsr82b1U4qvaRbSEjaeLHy8c4brDP"));

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

  return true;
}
