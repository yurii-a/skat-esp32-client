#include <string>
#include "config/save_config.h"
#include "SolanaSDK/keypair.h"
#include "SolanaSDK/signer.h"

void test_sign()
{
  unsigned char config_secret_key[64];

  getPrivateKey(config_secret_key);

  Keypair kp = Keypair(config_secret_key);
  Signer signer = Signer(kp);

  Serial.print("Public Key: ");
  Serial.println(kp.publicKey.toBase58().c_str());

  std::string signature = signer.sign("hello");
  Serial.print("Signature: ");
  Serial.println(signature.c_str());
}