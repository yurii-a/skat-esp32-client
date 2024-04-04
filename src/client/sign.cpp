#include <string>
#include "config/save_config.h"
#include "config/utils.h"
#include "SolanaSDK/keypair.h"
#include "SolanaSDK/signer.h"

void test_sign()
{
    String configSecretKey;

    getPrivateKey(configSecretKey);

    Keypair kp = Keypair(stringToKeypair(configSecretKey.c_str()));
  Signer signer = Signer(kp);

  Serial.print("Public Key: ");
  Serial.println(kp.publicKey.toBase58().c_str());

  std::string signature = signer.sign("hello");
  Serial.print("Signature: ");
  Serial.println(signature.c_str());
}