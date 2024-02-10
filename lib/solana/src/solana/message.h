#ifndef MESSAGE_H
#define MESSAGE_H

#include <cstdint>
#include <vector>
#include "./public_key.h"
#include "./hash.h"
#include "./instruction.h"

struct MessageHeader
{
  uint8_t numRequiredSignatures;
  uint8_t numReadonlySignedAccounts;
  uint8_t numReadonlyUnsignedAccounts;
};

class Message
{
public:
  // THe message header, identifying signed and read-only accountKeys
  MessageHeader header;

  // All the account keys used by the transaction
  std::vector<PublicKey> accountKeys;

  // The id of a recent ledger entry
  Hash recentBlockhash;

  // Programs the will be executed in sequence and committed in
  // one atomic transaction if all succeed
  std::vector<CompiledInstruction> instructions;

  void sanitize();

  Message(MessageHeader header, std::vector<PublicKey> accountKeys, Hash recentBlockhash, std::vector<CompiledInstruction> instructions);

  Message(std::vector<Instruction> instructions, std::optional<PublicKey> payer);

  static Message newWithBlockhash(std::vector<Instruction> instructions, std::optional<PublicKey> payer, Hash blockhash);

  Message newWithNonce(
      std::vector<Instruction> instructions,
      std::optional<PublicKey> payer,
      PublicKey &nonceAccountPublicKey,
      PublicKey &nonceAuthorityPublicKey);

  static Message newWithCompiledInstructions(
      uint8_t numRequiredSignatures,
      uint8_t numReadonlySignedAccounts,
      uint8_t numReadonlyUnsignedAccounts,
      std::vector<PublicKey> accountKeys,
      Hash recentBlockhash,
      std::vector<CompiledInstruction> instructions);

  CompiledInstruction compileInstruction(Instruction &ix);

  
};

#endif // MESSAGE_H
