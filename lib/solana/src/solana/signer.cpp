#include <sodium/crypto_sign_ed25519.h>
#include <string>
#include "solana/signer.h"
#include "solana/keypair.h"
#include "solana/base58.h"

Signer::Signer(Keypair kp)
{
  this->keypair = kp;
}

std::string Signer::sign(const std::string &message)
{
  unsigned char signature[crypto_sign_ed25519_BYTES];
  std::string signature_base58;

  // Sign the message using the private key
  crypto_sign_ed25519_detached(signature, NULL, reinterpret_cast<const unsigned char *>(message.c_str()), message.size(), keypair.get_secret_key());

  // Convert the signature to base58
  signature_base58 = base58Encode(signature, crypto_sign_ed25519_BYTES);

  return signature_base58;
}
