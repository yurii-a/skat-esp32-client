#ifndef COMPILED_KEYS_H
#define COMPILED_KEYS_H

#include <map>
#include <vector>
#include <optional>
#include "./public_key.h"
#include "./instruction.h"
#include "./message.h"

struct CompiledKeyMeta
{
  bool is_signer;
  bool is_writable;
  bool is_invoked;
};

class CompiledKeys
{
public:
  std::optional<PublicKey> payer;
  std::map<PublicKey, CompiledKeyMeta> keyMetaMap;

  static CompiledKeys compile(const std::vector<Instruction> &instructions, const std::optional<PublicKey> &payer);

  std::pair<MessageHeader, std::vector<PublicKey>> tryIntoMessageComponents();
};

#endif // COMPILED_KEYS_H
