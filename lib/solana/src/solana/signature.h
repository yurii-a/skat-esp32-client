#ifndef SIGNATURE_H
#define SIGNATURE_H

#include <cstdint>
#include <vector>
#include <array>
#include <stdexcept>
#include <string>
#include "./keypair.h"
#include "./public_key.h"

constexpr size_t SIGNATURE_BYTES = 64;
constexpr size_t MAX_BASE58_SIGNATURE_LEN = 88;

class Signature
{
public:
    std::array<uint8_t, SIGNATURE_BYTES> value;

    Signature();
    Signature(const std::vector<uint8_t> &signature_slice);
    static Signature new_unique();
    bool verify(const std::vector<uint8_t> &pubkey_bytes, const std::vector<uint8_t> &message_bytes);
    std::string to_string() const;
    static Signature from_string(const std::string &s);

    uint8_t operator*() const
    {
        return value[0];
    }

    bool operator!=(const Signature &other) const
    {
        return value != other.value;
    }
};

std::ostream &operator<<(std::ostream &os, const Signature &signature);

class Signable
{
public:
    virtual void sign(const Keypair &keypair) = 0;
    virtual bool verify() const = 0;
    virtual PublicKey pubkey() const = 0;
    virtual std::vector<uint8_t> signable_data() const = 0;
    virtual Signature get_signature() const = 0;
    virtual void set_signature(const Signature &signature) = 0;
};

#endif // SIGNATURE_H
