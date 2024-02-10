#include "./public_key.h"

class AccountMeta
{
public:
    // An account's public key
    PublicKey publicKey;

    // True if an Instruction required a Transaction signature matching public_key
    bool isSigner;

    // True if the account data or metadata maybe mutated during program execution
    bool isWritable;

    // Construct metadata for a writable account.
    static AccountMeta *newWritable(PublicKey publicKey, bool isSigner)
    {
        AccountMeta *accountMeta = new AccountMeta();
        accountMeta->publicKey = publicKey;
        accountMeta->isSigner = isSigner;
        accountMeta->isWritable = true;
        return accountMeta;
    }

    // Construct metadata for a read-only account.
    static AccountMeta *newReadonly(PublicKey publicKey, bool isSigner)
    {
        AccountMeta *accountMeta = new AccountMeta();
        accountMeta->publicKey = publicKey;
        accountMeta->isSigner = isSigner;
        accountMeta->isWritable = false;
        return accountMeta;
    }
};