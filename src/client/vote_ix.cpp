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
// #include <nlohmann/json.hpp>
// using json = nlohmann::json;


std::vector<uint8_t> bufferToHexUint8Vector_vote(std::vector<uint8_t> buffer) {
    std::vector<uint8_t> result;
    for (size_t i = 0; i < buffer.size(); ++i) {
        result.push_back(static_cast<uint8_t>(buffer[i]));
    }
    return result;
}
// 0 = init
// 1 = trump
// 2 = biden 
bool vote_ix(int x)
{
  const char *devnetRPC = "https://parental-ardenia-fast-devnet.helius-rpc.com/";
  const char *mainnetRPC = "https://cecily-q1u5dh-fast-mainnet.helius-rpc.com/";
  Serial.println("### INPUT:   ");

  Serial.println(x);

  // Establish WiFi connection (code from previous snippet)

  // Make the REST API call
  if (WiFi.status() == WL_CONNECTED)
  {

    // http get request for ix data from frontend


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
    Signer data_signer = Signer(data);
    PublicKey voteAccount = PublicKey(Base58::trimDecode("GHfSeQis56LEeXURnGLWukB8LdvTdgs4wxDB9rN7usQi"));

    AccountMeta *payerAccountMeta = AccountMeta::newWritable(signer.publicKey(), true);
    // AccountMeta *voteAccountMeta = AccountMeta::newWritable(data_signer.publicKey(), true);
    AccountMeta *voteAccountMeta = AccountMeta::newWritable(voteAccount, false);

    PublicKey systemProgramId = SystemProgram::id();
    AccountMeta *systemProgramAccountMeta = AccountMeta::newReadonly(systemProgramId, false);
    
    std::vector<uint8_t> transferData;
    std::vector<AccountMeta> accounts;
    

    std::vector<uint8_t> initData = bufferToHexUint8Vector_vote({55,186,159,175,72,116,84,79});
    std::vector<uint8_t> voteTrumpData = bufferToHexUint8Vector_vote({175,175,109,31,13,152,155,237});
    std::vector<uint8_t> voteBidenData = bufferToHexUint8Vector_vote({152,120,185,205,231,78,197,224});

    // std::vector<uint8_t> transferData = {0xaf, 0xaf, 0x6d, 0x1f, 0x0d, 0x98, 0x9b, 0xed, 0x09, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};+

    std::vector<AccountMeta> init_accounts = {*payerAccountMeta,*voteAccountMeta,  *systemProgramAccountMeta};
    std::vector<AccountMeta> vote_accounts = {*voteAccountMeta};

    if (x==0){
        transferData = initData;
        accounts = init_accounts;
    } else if (x==1) {
        transferData = voteTrumpData;
        accounts = vote_accounts;
    } else if (x==2) {
        transferData = voteBidenData;
        accounts = vote_accounts;
    }

    PublicKey voteProgramId = PublicKey(Base58::trimDecode("2tLziWPq81Yey2kNsFGkSnkjcGpmhREatcJBxgYvkHun"));

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
    // signerVector.push_back(signer);
    // signerVector.push_back(data_signer);

    Signers signers(signerVector);

    tx.sign(signers, recentBlockhash.blockhash);

    std::vector<uint8_t> serializedTx = tx.serialize();
    for (auto byte : serializedTx) {
      Serial.print(byte);
      Serial.print(",");
    }
    Serial.println(" ");

    Signature returnedSignature = connection.sendTransaction(tx);

    Serial.print("SIGNATURE: ");
    Serial.println(returnedSignature.toString().c_str());
  }

  return true;
}

