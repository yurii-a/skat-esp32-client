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

bool sendBid()
{
  const char *rpc = "https://api-devnet.helius.xyz";
//   const char *rpc = "https://relevant-lebbie-fast-mainnet.helius-rpc.com/";

  // Establish WiFi connection (code from previous snippet)

  // Make the REST API call
  if (WiFi.status() == WL_CONNECTED)
  {
    HTTPClient http;
    // http.begin(rpc);
    http.begin(rpc);

    Commitment commitment = Commitment::finalized;

    // Connection connection = Connection(rpc, commitment);
    Connection connection = Connection(rpc, commitment);

    BlockhashWithExpiryBlockHeight recentBlockhash = connection.getLatestBlockhash();

    // Get secret key from saved config
    String configSecretKey;

    getPrivateKey(configSecretKey);

    Keypair kp = Keypair(stringToKeypair(configSecretKey.c_str()));
    Signer signer = Signer(kp);

    String sender = Base58::trimEncode(signer.publicKey().serialize()).c_str();
    Serial.print("SENDER: ");
    Serial.println(sender);

    // BqWi2aZYQRUk9ZGMD9Ti71bkVhngsg7SubiJ81Eznqoa
    // GxCJ5iHEbiNu3WYpaRdFrQde7oiRcg5PCSbJYjRq2XEy
    PublicKey recipient = PublicKey(Base58::trimDecode("7Eh3n61XxkVYosxFBLUWDG6PJCeUMNz8L982n5cqzCUx"));
    Serial.print("RECIPIENT: ");
    Serial.println(recipient.toBase58().c_str());

    // BhUhTyLSR6B3zjXv7gCHEKSs8WQ8wX5BvKB4gPHxnAZU
    // 7n3bAL35wfH58iqaNx22qjnBqAC4dYaWwkxUVNPZZJoN
    // 7wxVFNuK35shzgrF1wjdrTmfkUd3SSkRCXrZqxYjQVDX
    PublicKey sourceAta = PublicKey(Base58::trimDecode("CsmrdapyDmPkUjSbPtQkCwbcRDR5XyEaDoXoh2NP8RB5"));
    Serial.print("SOURCE ATA: ");
    Serial.println(sourceAta.toBase58().c_str());

    // GTSPJDQmYWKEz6mtfMcCJgCh42ac2Sxqg3BGMRM4GGzV
    // AYSwG6QVAJw8aCM8pcDYnAKCrfPtdjcY3v3ciBEmyjwJ
    // 7Eh3n61XxkVYosxFBLUWDG6PJCeUMNz8L982n5cqzCUx
    PublicKey recipientAta = PublicKey(Base58::trimDecode("AKDpTf2AvT8gXGcW6HL5G3A6oBG2CAY7icbuAUfwpa7A"));
    Serial.print("RECIPIENT ATA: ");
    Serial.println(recipientAta.toBase58().c_str());

    // ERkzwkBkPeF1Q2LJr9Jqr2dcy4zZSV1pX6VGFxDmniM2
    // DezXAZ8z7PnrnRJjz3wXBoRgixCa6xjnB7YaB1pPB263
    PublicKey mint = PublicKey(Base58::trimDecode("ERkzwkBkPeF1Q2LJr9Jqr2dcy4zZSV1pX6VGFxDmniM2"));
    Serial.print("MINT: ");
    Serial.println(mint.toBase58().c_str());

    PublicKey tokenProgram = PublicKey(Base58::trimDecode("TokenkegQfeZyiNwAJbNbGKPFXCWuBvf9Ss623VQ5DA"));

    PublicKey computeBudgetProgram = PublicKey(Base58::trimDecode("ComputeBudget111111111111111111111111111111"));

    AccountMeta *sourceAtaMeta = AccountMeta::newWritable(sourceAta, false);
    AccountMeta *mintMeta = AccountMeta::newReadonly(mint, false);
    AccountMeta *recipientAtaMeta = AccountMeta::newWritable(recipientAta, false);
    AccountMeta *senderMeta = AccountMeta::newWritable(signer.publicKey(), true);
    // std::vector<uint8_t> transferData = {
    //     0x0c, 0x80, 0x96, 0x98, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06};
    std::vector<uint8_t> transferData = {
        0x0c, 0x00, 0xe4, 0x0b, 0x54, 0x02, 0x00, 0x00, 0x00, 0x06};

    std::vector<uint8_t> computeBudgetData = {
        0x03,
        0x7f,
        0x1f,
        0x16,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
    };

    std::vector<AccountMeta> accounts = {
        *sourceAtaMeta,
        *mintMeta,
        *recipientAtaMeta,
        *senderMeta,
    };

    Instruction cupIx = Instruction::newWithBytes(
        computeBudgetProgram, computeBudgetData,
        {});

    Instruction ix = Instruction::newWithBytes(
        tokenProgram, transferData,
        accounts);

    std::vector<Instruction> ixs;
    ixs.push_back(cupIx);
    ixs.push_back(ix);

    std::optional<PublicKey> payer;

    payer = signer.publicKey();

    Message msg = Message::newWithBlockhash(ixs, payer, recentBlockhash.blockhash);

    Transaction tx = Transaction(msg);

    std::vector<Signer> signerVector;
    signerVector.push_back(signer);

    Signers signers(signerVector);

    tx.sign(signers, recentBlockhash.blockhash);

    // for (auto byte : msg.serialize())
    // {
    //   Serial.print(byte);
    //   Serial.print(",");
    // }
    // Serial.println("");

    // for (auto byte : tx.serialize())
    // {
    //   Serial.print(byte);
    //   Serial.print(",");
    // }
    // Serial.println("");

    Serial.println(base64::encode(tx.serialize().data(), tx.serialize().size()));

    SendOptions sendOptions = {
        false,
        Commitment::finalized,
        5,
    };

    Signature returnedSignature = connection.sendTransaction(tx, sendOptions);

    Serial.print("SIGNATURE: ");
    Serial.println(returnedSignature.toString().c_str());

    Serial.println("10,000 BONK Burned!!!!");

    http.end();
  }

  return true;
}
