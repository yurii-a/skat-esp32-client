#include <cstdint>
#include <vector>
#include "./public_key.h"
#include "./hash.h"
#include "./instruction.h"
#include "./compiled_keys.h"
#include "./message.h"

void Message::sanitize()
{
  // signing area and read--only on-signing area should no overlap
  if (this->header.numRequiredSignatures + this->header.numReadonlyUnsignedAccounts > this->accountKeys.size())
  {
    throw std::out_of_range("Number of required signatures plus read-only unsigned accounts exceeds total number of account keys");
  }

  // there should be at least 1 RW fee-payer account
  if (this->header.numReadonlySignedAccounts >= this->header.numRequiredSignatures)
  {
    throw std::out_of_range("Need at least 1 read-only signed account");
  }

  for (const auto &ci : this->instructions)
  {
    if (ci.program_id_index >= this->accountKeys.size())
    {
      throw std::out_of_range("Invalid program id index");
    }

    // A program cannot be a payer
    if (ci.program_id_index == 0)
    {
      throw std::out_of_range("Program cannot be a payer");
    }

    for (const auto &ai : ci.accounts)
    {
      if (ai >= this->accountKeys.size())
      {
        throw std::out_of_range("Invalid account keys size");
      }
    }
  }

  for (auto &ak : this->accountKeys)
  {
    ak.sanitize();
  }

  this->recentBlockhash.sanitize();

  for (auto &ix : this->instructions)
  {
    ix.sanitize();
  }
}

Message::Message(MessageHeader header, std::vector<PublicKey> accountKeys, Hash recentBlockhash, std::vector<CompiledInstruction> instructions)
    : header(header), accountKeys(accountKeys), recentBlockhash(recentBlockhash), instructions(instructions) {}

Message::Message(std::vector<Instruction> instructions, std::optional<PublicKey> payer)
{
  Message::newWithBlockhash(instructions, payer, Hash());
}

Message Message::newWithNonce(
    std::vector<Instruction> instructions,
    std::optional<PublicKey> payer,
    PublicKey &nonceAccountPublicKey,
    PublicKey &nonceAuthorityPublicKey)
{
  // TODO: add advance nonce account instruction
  Message(instructions, payer);
}

Message Message::newWithBlockhash(std::vector<Instruction> instructions, std::optional<PublicKey> payer, Hash blockhash)
{
  CompiledKeys compiledKeys = CompiledKeys::compile(instructions, payer);
  MessageHeader header;
  std::vector<PublicKey> accountKeys;
  std::tie(header, accountKeys) = compiledKeys.tryIntoMessageComponents();

  std::vector<CompiledInstruction> ixs = compileInstructions(instructions, accountKeys);

  return Message::newWithCompiledInstructions(
      header.numRequiredSignatures,
      header.numReadonlySignedAccounts,
      header.numReadonlyUnsignedAccounts,
      accountKeys,
      blockhash,
      ixs);
}

Message Message::newWithCompiledInstructions(
    uint8_t numRequiredSignatures,
    uint8_t numReadonlySignedAccounts,
    uint8_t numReadonlyUnsignedAccounts,
    std::vector<PublicKey> accountKeys,
    Hash recentBlockhash,
    std::vector<CompiledInstruction> instructions)
{
  MessageHeader header = MessageHeader{
      numRequiredSignatures,
      numReadonlySignedAccounts,
      numReadonlyUnsignedAccounts};

  Message message = Message{
      header,
      accountKeys,
      recentBlockhash,
      instructions};
  return message;
}

CompiledInstruction Message::compileInstruction(Instruction &ix)
{
  return compileIx(ix, this->accountKeys);
}