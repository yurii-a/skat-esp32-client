#ifndef SIGNER_H
#define SIGNER_H

#include <sodium/crypto_sign_ed25519.h>
#include <string>
#include "solana/keypair.h"
#include "solana/base58.h"

class Signer {
private:
    Keypair keypair;

public:
    Signer(Keypair kp);

    std::string sign(const std::string& message);
};

#endif // SIGNER_H
