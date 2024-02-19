#ifndef HASH_H
#define HASH_H

#include <cstring>
#include <array>
#include <vector>
#include <Crypto.h>

// Size of hash in bytes
constexpr size_t HASH_BYTES = 32;
// Maximum string length of a base58 encoded hash.
constexpr size_t MAX_BASE58_LEN = 44;

class Hash
{
public:
    std::array<uint8_t, HASH_BYTES> data;

    Hash();
    Hash(std::array<uint8_t, HASH_BYTES> &hashArray);
    static Hash newFromArray(std::array<uint8_t, HASH_BYTES> hashArray);
    static Hash newUnique();
    std::array<uint8_t, HASH_BYTES> toBytes() const;
    void sanitize();
    std::vector<uint8_t> serialize();
    static Hash deserialize(const std::vector<uint8_t> &input);

    bool operator!=(const Hash &other) const
    {
        return data != other.data;
    }

    Hash operator+(const Hash &other) const
    {
        Hash result;
        for (size_t i = 0; i < HASH_BYTES; i++)
        {
            result.data[i] = this->data[i] ^ other.data[i];
        }
        return result;
    }
};

class Hasher
{
public:
    SHA256 hasher;

    void hash(const uint8_t *val, size_t len);
    void hashv(const std::vector<uint8_t *> &vals);
    void result(Hash *hash);
};

#endif // HASH_H