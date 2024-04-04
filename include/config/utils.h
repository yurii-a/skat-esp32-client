#include "SolanaSDK/keypair.h"

std::string keypairToString(Keypair keypair);

Keypair stringToKeypair(const std::string &encodedString);

void stringToHexArray(const String &str, uint8_t *hexArray, int &size);

void reverseVector(std::vector<uint8_t> &vec);