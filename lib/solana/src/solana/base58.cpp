#include <string>
#include <vector>

const char *const ALPHABET =
    "123456789ABCDEFGHJKLMNPQRSTUVWXYZabcdefghijkmnopqrstuvwxyz";
const signed char ALPHABET_MAP[256] = {
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    0,
    1,
    2,
    3,
    4,
    5,
    6,
    7,
    8,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    9,
    10,
    11,
    12,
    13,
    14,
    15,
    16,
    -1,
    17,
    18,
    19,
    20,
    21,
    -1,
    22,
    23,
    24,
    25,
    26,
    27,
    28,
    29,
    30,
    31,
    32,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    33,
    34,
    35,
    36,
    37,
    38,
    39,
    40,
    41,
    42,
    43,
    -1,
    44,
    45,
    46,
    47,
    48,
    49,
    50,
    51,
    52,
    53,
    54,
    55,
    56,
    57,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
};

const double iFactor = 1.36565823730976103695740418120764243208481439700722980119458355862779176747360903943915516885072037696111192757109;

std::string base58Encode(const unsigned char *source, int len)
{
    int zeros = 0, length = 0, pbegin = 0, pend;
    if (!(pend = len))
        return "";
    while (pbegin != pend && !source[pbegin])
        pbegin = ++zeros;
    int size = 1 + iFactor * (double)(pend - pbegin);
    unsigned char b58[size];
    for (int i = 0; i < size; i++)
        b58[i] = 0;
    while (pbegin != pend)
    {
        unsigned int carry = source[pbegin];
        int i = 0;
        for (int it1 = size - 1; (carry || i < length) && (it1 != -1); it1--, i++)
        {
            carry += 256 * b58[it1];
            b58[it1] = carry % 58;
            carry /= 58;
        }
        if (carry)
            return "";
        length = i;
        pbegin++;
    }
    int it2 = size - length;
    while ((it2 - size) && !b58[it2])
        it2++;
    std::string result(zeros + size - it2 + 1, '\0');
    int ri = 0;
    while (ri < zeros)
        result[ri++] = '1';
    for (; it2 < size; ++it2)
        result[ri++] = ALPHABET[b58[it2]];
    result[ri] = '\0';
    return result;
}

std::vector<int> base58Decode(const unsigned char *str, int len)
{
    std::vector<int> result;
    result.push_back(0);

    for (int i = 0; i < len; i++)
    {
        unsigned int carry = (unsigned int)ALPHABET_MAP[str[i]];
        if (carry == -1)
        {
            // Return an empty vector on error
            return std::vector<int>();
        }

        for (int j = 0; j < result.size(); j++)
        {
            carry += (unsigned int)(result[j]) * 58;
            result[j] = carry % 256;
            carry /= 256;
        }

        while (carry > 0)
        {
            result.push_back(carry % 256);
            carry /= 256;
        }
    }

    for (int i = 0; i < len && str[i] == '1'; i++)
        result.push_back(0);

    for (int i = result.size() - 1, z = (result.size() >> 1) + (result.size() & 1);
         i >= z; i--)
    {
        int k = result[i];
        result[i] = result[result.size() - i - 1];
        result[result.size() - i - 1] = k;
    }

    return result;
}