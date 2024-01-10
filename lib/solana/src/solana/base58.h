#include <string>
#include <vector>

std::string base58_encode(const unsigned char *source, int len) ;

std::vector<int> base58_decode(const unsigned char *str, int len);