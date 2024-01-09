#include "sodium/crypto_sign_ed25519.h"
#include <config/saveConfig.h>
#include <bip39/bip39.h>

void getKeypairFromSeed() {
  unsigned char privateKey[crypto_sign_ed25519_SEEDBYTES];
  unsigned char pk[crypto_sign_ed25519_PUBLICKEYBYTES];
  unsigned char sk[crypto_sign_ed25519_SECRETKEYBYTES];

  getPrivateKey(privateKey);

  crypto_sign_ed25519_seed_keypair(pk, sk, privateKey);

  for (int i = 0; i < crypto_sign_ed25519_PUBLICKEYBYTES; i++) {
    Serial.println(pk[i]);
  }
    Serial.println("-----");
  for (int i = 0; i < crypto_sign_ed25519_SECRETKEYBYTES; i++) {
    Serial.println(sk[i]);
  }
}