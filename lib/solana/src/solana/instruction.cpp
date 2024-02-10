#include <cstdint>
#include <vector>
#include <algorithm>
#include <stdexcept>
#include "./public_key.h"
#include "./account_meta.h"

class Instruction
{
public:
    // PublicKey of the program that executes this instruction
    PublicKey program_id;

    // Metadata describing accounts that should be passed to the program
    std::vector<AccountMeta> accounts;

    // Opaque data passed to the program for its own interpretation
    std::vector<uint8_t> data;
};

class CompiledInstruction
{
public:
    // Index into the transaction keys array indicating the program account that executes this instruction.
    uint8_t program_id_index;

    // Ordered indices into the transaction keys array indicating which accounts to pass to the program.
    std::vector<uint8_t> accounts;

    // The program input data.
    std::vector<uint8_t> data;

    template <typename T>
    CompiledInstruction(uint8_t program_ids_index, const T &data, std::vector<uint8_t> accounts)
        : program_id_index(program_ids_index), data(data), accounts(std::move(accounts)) {}

    CompiledInstruction(uint8_t program_id_index, std::vector<uint8_t> data, std::vector<uint8_t> accounts)
        : program_id_index(program_id_index), data(std::move(data)), accounts(std::move(accounts)) {}

    const PublicKey &program_id(const std::vector<PublicKey> &program_ids) const
    {
        return program_ids[program_id_index];
    }

    void sanitize() {}
};

uint8_t position(const std::vector<PublicKey> &keys, const PublicKey &key)
{
    auto it = std::find(keys.begin(), keys.end(), key);
    if (it == keys.end())
    {
        throw std::runtime_error("Key not found");
    }
    return std::distance(keys.begin(), it);
}

CompiledInstruction compileIx(const Instruction &ix, const std::vector<PublicKey> &keys)
{
    std::vector<uint8_t> accounts;
    for (const auto &accountMeta : ix.accounts)
    {
        accounts.push_back(position(keys, accountMeta.publicKey));
    }
    return CompiledInstruction{position(keys, ix.program_id), ix.data, accounts};
}

std::vector<CompiledInstruction> compileInstructions(const std::vector<Instruction> &ixs, const std::vector<PublicKey> &keys)
{
    std::vector<CompiledInstruction> compiled;
    for (const auto &ix : ixs)
    {
        compiled.push_back(compileIx(ix, keys));
    }
    return compiled;
}