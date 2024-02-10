#ifndef PUBLIC_KEY_H
#define PUBLIC_KEY_H

#include <string>
#include "solana/base58.h"

const uint8_t PUBLIC_KEY_LEN = 32;

class PublicKey
{
public:
    unsigned char key[PUBLIC_KEY_LEN];

    // Default constructor
    PublicKey();

    // Parameterized constructor
    PublicKey(const unsigned char value[PUBLIC_KEY_LEN]);

    // Convert key to base58
    std::string toBase58();

    // sanitize
    void sanitize();

    // Less-than operator
    bool operator<(const PublicKey &other) const
    {
        return std::lexicographical_compare(key, key + PUBLIC_KEY_LEN, other.key, other.key + PUBLIC_KEY_LEN);
    }

    // Equality operator
    bool operator==(const PublicKey &other) const
    {
        return std::equal(key, key + PUBLIC_KEY_LEN, other.key);
    }
};

#endif // PUBLIC_KEY_H
