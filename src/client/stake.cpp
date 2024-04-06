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
// 1 = deposit
// 2 = burn
//test only deposit for now

bool stake(int x)
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
    PublicKey vaultAccount = PublicKey(Base58::trimDecode("AkxNG3CetDpC4a31hfVuGmLEQjpW8rQYwxisky2wAVRe"));
    PublicKey stateAccount = PublicKey(Base58::trimDecode("ATge3qYHxjYeaYx4H9W94t4uiELr5d5CbWTkZk3SQsvx"));


    AccountMeta *payerAccountMeta = AccountMeta::newWritable(signer.publicKey(), true);
    // AccountMeta *voteAccountMeta = AccountMeta::newWritable(data_signer.publicKey(), true);
    AccountMeta *vaultAccountMeta = AccountMeta::newWritable(vaultAccount, false);

    AccountMeta *stateAccountMeta = AccountMeta::newReadonly(stateAccount, false);


    PublicKey systemProgramId = SystemProgram::id();
    AccountMeta *systemProgramAccountMeta = AccountMeta::newReadonly(systemProgramId, false);
    
    std::vector<uint8_t> transferData;
    std::vector<AccountMeta> accounts;
    

    std::vector<uint8_t> initData = bufferToHexUint8Vector_vote({175,175,109,31,13,152,155,237});
    std::vector<uint8_t> depositData = bufferToHexUint8Vector_vote({242,35,198,137,82,225,242,182,0,202,154,59,0,0,0,0});
    std::vector<uint8_t> withdrawData = bufferToHexUint8Vector_vote({183,18,70,156,148,109,161,34,0,202,154,59,0,0,0,0});

    // std::vector<uint8_t> transferData = {0xaf, 0xaf, 0x6d, 0x1f, 0x0d, 0x98, 0x9b, 0xed, 0x09, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};+

    std::vector<AccountMeta> init_accounts = {*payerAccountMeta,*vaultAccountMeta,  *systemProgramAccountMeta};
    std::vector<AccountMeta> payments_accounts = {*payerAccountMeta, *stateAccountMeta, *vaultAccountMeta, *systemProgramAccountMeta};
    // ignore x=0 case for now
    if (x==0){
        transferData = initData;
        accounts = init_accounts;
    } else if (x==1) {
        transferData = depositData;
        accounts = payments_accounts;
    } else if (x==2) {
        transferData = withdrawData;
        accounts = payments_accounts;
    }

    PublicKey voteProgramId = PublicKey(Base58::trimDecode("A43P8qojkZTjkCDL2VPJG4VFKbgi7xTMyRWK6fHEQnJE"));

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

