#ifndef COMPILED_INSTRUCTION_H
#define COMPILED_INSTRUCTION_H

#include <cstdint>
#include <vector>
#include "./public_key.h"
#include "./account_meta.h"

class Instruction
{
public:
    PublicKey program_id;
    std::vector<AccountMeta> accounts;
    std::vector<uint8_t> data;
};

class CompiledInstruction
{
public:
    uint8_t program_id_index;
    std::vector<uint8_t> accounts;
    std::vector<uint8_t> data;

    template <typename T>
    CompiledInstruction(uint8_t program_ids_index, const T &data, std::vector<uint8_t> accounts);
    CompiledInstruction(uint8_t program_id_index, std::vector<uint8_t> data, std::vector<uint8_t> accounts);
    const PublicKey &program_id(const std::vector<PublicKey> &program_ids) const;
    void sanitize();
};

uint8_t position(const std::vector<PublicKey> &keys, const PublicKey &key);
CompiledInstruction compileIx(const Instruction &ix, const std::vector<PublicKey> &keys);
std::vector<CompiledInstruction> compileInstructions(const std::vector<Instruction> &ixs, const std::vector<PublicKey> &keys);

#endif // COMPILED_INSTRUCTION_H
