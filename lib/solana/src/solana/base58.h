#include <string>
#include <vector>

std::string base58Encode(const unsigned char *source, int len);

std::vector<int> base58Decode(const unsigned char *str, int len);