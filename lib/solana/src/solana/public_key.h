#ifndef PUBLIC_KEY_H
#define PUBLIC_KEY_H

#include <string>
#include "solana/base58.h"

const uint8_t PUBLIC_KEY_LEN = 32;

class PublicKey {
public:
    unsigned char key[PUBLIC_KEY_LEN];

    // Default constructor
    PublicKey();

    // Parameterized constructor
    PublicKey(const unsigned char value[PUBLIC_KEY_LEN]);

    // Convert key to base58
    std::string to_base58();
};

#endif // PUBLIC_KEY_H
