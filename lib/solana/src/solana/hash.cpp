#include <atomic>
#include <sstream>
#include <iomanip>
#include "Hash.h"

Hash::Hash()
{
    this->data.fill(0);
}

Hash::Hash(std::array<uint8_t, HASH_BYTES> &hashArray)
{
    this->data = hashArray;
}

// Constructor to initialize from an array
Hash Hash::newFromArray(std::array<uint8_t, HASH_BYTES> hashArray)
{
    return Hash(hashArray);
}

// Static method to create a unique Hash
Hash Hash::newUnique()
{
    static uint64_t I = 1;
    std::array<uint8_t, HASH_BYTES> b{};
    std::memcpy(b.data(), &I, sizeof(uint64_t));
    I++;
    return Hash(b);
}

// Method to convert the Hash to an array of bytes
std::array<uint8_t, HASH_BYTES> Hash::toBytes() const
{
    return data;
}

void Hash::sanitize() {}

// Serialize method
std::vector<uint8_t> Hash::serialize()
{
    std::vector<uint8_t> result(data.begin(), data.end());
    return result;
}

// Deserialize method
Hash Hash::deserialize(const std::vector<uint8_t> &input)
{
    if (input.size() != HASH_BYTES)
    {
        throw std::invalid_argument("Invalid hash string");
    }
    Hash hash;
    for (size_t i = 0; i < HASH_BYTES; ++i)
    {
        hash.data[i] = input[i];
    }
    return hash;
}

void Hasher::hash(const uint8_t *val, size_t len)
{
    this->hasher.doUpdate(val, len);
}

void Hasher::hashv(const std::vector<uint8_t *> &vals)
{
    for (auto &val : vals)
    {
        this->hash(val, std::strlen(reinterpret_cast<const char *>(val)));
    }
}

void Hasher::result(Hash *hash)
{
    this->hasher.doFinal(reinterpret_cast<byte *>(hash->data.data()));
}
