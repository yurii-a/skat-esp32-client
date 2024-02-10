#include <cstdint>
#include <vector>
#include <array>
#include <stdexcept>
#include <string>
#include <sstream>
#include <iomanip>
#include "./keypair.h"
#include "./public_key.h"

// Number of bytes in a signature
constexpr size_t SIGNATURE_BYTES = 64;
// Maximum string length of a base58 encoded signature
constexpr size_t MAX_BASE58_SIGNATURE_LEN = 88;

class Signature {
public:
    std::array<uint8_t, SIGNATURE_BYTES> value;

    Signature() : value{} {}

    Signature(const std::vector<uint8_t>& signature_slice) {
        if (signature_slice.size() != SIGNATURE_BYTES) {
            throw std::invalid_argument("Wrong size for signature");
        }
        std::copy(signature_slice.begin(), signature_slice.end(), value.begin());
    }

    static Signature new_unique() {
        Signature signature;
        for (auto& byte : signature.value) {
            byte = rand() % 256;  // Replace with a better random generator if needed
        }
        return signature;
    }

    bool verify(const std::vector<uint8_t>& pubkey_bytes, const std::vector<uint8_t>& message_bytes) {
        // Implement the verification logic here
        return false;  // Placeholder
    }

    std::string to_string() const {
        std::ostringstream oss;
        for (const auto& byte : value) {
            oss << std::setw(2) << std::setfill('0') << std::hex << static_cast<int>(byte);
        }
        return oss.str();
    }

    static Signature from_string(const std::string& s) {
        if (s.size() > MAX_BASE58_SIGNATURE_LEN) {
            throw std::invalid_argument("Wrong size for signature");
        }
        // Implement the base58 decoding here
        return Signature();  // Placeholder
    }
};

std::ostream& operator<<(std::ostream& os, const Signature& signature) {
    return os << signature.to_string();
}

class Signable {
public:
    virtual void sign(const Keypair& keypair) = 0;
    virtual bool verify() const = 0;
    virtual PublicKey pubkey() const = 0;
    virtual std::vector<uint8_t> signable_data() const = 0;
    virtual Signature get_signature() const = 0;
    virtual void set_signature(const Signature& signature) = 0;
};
