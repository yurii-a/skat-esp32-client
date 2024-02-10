#include <vector>
#include <optional>
#include <utility>
#include "./signature.h"
#include "./message.h"
#include "./instruction.h"
#include "./compiled_keys.h"

class Transaction
{
public:
  // A set of signatures of a serialized Message, signed by the first
  // keys of the Message's accountKeys, where the number of signatures
  // is equal to numRequiredSignatures of the Message's MessageHeader
  std::vector<Signature> signatures;

  // the message to sign
  Message message;

  void sanitize()
  {
    if (this->message.header.numRequiredSignatures > this->signatures.size())
    {
      throw std::out_of_range("Number of required signatures exceeds the number of signatures");
    }
    if (this->signatures.size() > this->message.accountKeys.size())
    {
      throw std::out_of_range("Number of signatures exceeds the number of account keys");
    }
    this->message.sanitize();
  }
};