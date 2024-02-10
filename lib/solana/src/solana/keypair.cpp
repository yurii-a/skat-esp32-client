#include <sodium/crypto_sign_ed25519.h>
#include <string>
#include <vector>
#include <algorithm>
#include <cassert>
#include "solana/keypair.h"
#include "solana/base58.h"
#include "solana/public_key.h"
#include "esp_random.h"

// Default constructor for zero-initialized keys
Keypair::Keypair()
{
    std::fill(secret_key, secret_key + SECRET_KEY_LEN, 0);
}

// Generate keypair from a secure seed
Keypair::Keypair(const std::vector<unsigned char> &seed)
{
    assert(seed.size() == SECRET_KEY_LEN); // Enforce valid seed size
    unsigned char pub_key[PUBLIC_KEY_LEN];
    crypto_sign_ed25519_seed_keypair(pub_key, this->secret_key, seed.data());
    this->public_key = PublicKey(pub_key);
}

// Generate keypair from a secure seed (convenience for C arrays)
Keypair::Keypair(const unsigned char seed[SECRET_KEY_LEN]) : Keypair(std::vector<unsigned char>(seed, seed + SECRET_KEY_LEN)) {}

// Access secret key with authorization
const unsigned char *Keypair::get_secret_key()
{
    return this->secret_key;
}

// Destructor to securely clear secret key
Keypair::~Keypair()
{
    std::fill(secret_key, secret_key + SECRET_KEY_LEN, 0);
}

// Generate a new Keypair with a random seed
Keypair Keypair::generate()
{
    std::vector<unsigned char> seed(SECRET_KEY_LEN);
    esp_fill_random(&seed, SECRET_KEY_LEN);
    return Keypair(seed);
}
