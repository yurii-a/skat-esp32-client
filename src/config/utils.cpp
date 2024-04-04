#include <Arduino.h>
#include "SolanaSDK/keypair.h"
#include "SolanaSDK/base58.h"

std::string keypairToString(Keypair keypair)
{
  const unsigned char *k = keypair.getSecretKey();

  std::vector<uint8_t> input(k, k + SECRET_KEY_LEN); // Initialize vector with secret key bytes

  // Encoding the byte array to Base58 using the provided Base58 class
  std::string encoded = Base58::trimEncode(input);

  return encoded;
}

Keypair stringToKeypair(const std::string &encodedString)
{
  std::vector<uint8_t> decodedBytes = Base58::trimDecode(encodedString);

  // Create a Keypair object using the decoded bytes
  Keypair keypair(decodedBytes.data());

  return keypair;
}

void stringToHexArray(const String& str, uint8_t* hexArray, int& size) {
  size = str.length();
  for (int i = 0; i < size; i++) {
    hexArray[i] = str.charAt(i);
  }
}

void reverseVector(std::vector<uint8_t>& vec) {
    size_t left = 0;
    size_t right = vec.size() - 1;
    while (left < right) {
        uint8_t temp = vec[left];
        vec[left] = vec[right];
        vec[right] = temp;
        left++;
        right--;
    }
}