#include <string>
#include "solana/public_key.h"
#include "solana/base58.h"
#include "public_key.h"

PublicKey::PublicKey()
{
  std::fill(key, key + PUBLIC_KEY_LEN, 0);
}

PublicKey::PublicKey(const unsigned char value[PUBLIC_KEY_LEN])
{
  std::copy(value, value + PUBLIC_KEY_LEN, this->key);
}

std::string PublicKey::toBase58()
{
  return base58Encode(this->key, PUBLIC_KEY_LEN);
}

void PublicKey::sanitize() {}
