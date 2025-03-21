
// test/test_anchor/test_anchor_transaction.cpp
#include <unity.h>
#include <WiFi.h>
#include "SolanaSDK/connection.h"
#include "SolanaSDK/transaction.h"

// Assume these are defined elsewhere or in config
extern const char* ssid;
extern const char* password;
extern const char* devnetRPC;

void setUp(void) {
    // Connect to WiFi before each test
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
    }
}

void tearDown(void) {
    WiFi.disconnect();
}

void test_anchor_transaction_completion(void) {
    Connection connection(devnetRPC, Commitment::confirmed);
    std::vector<Instruction> instructions; // Add your Anchor program instruction here
    PublicKey feePayer("YourPublicKeyHere"); // Replace with real key
    BlockhashWithExpiryBlockHeight blockhash = connection.getLatestBlockhash();
    Message msg = Message::newWithBlockhash(instructions, feePayer, blockhash.blockhash);
    Transaction tx(msg);

    // Send transaction
    Signature signature = connection.sendTransaction(tx);

    // Wait for confirmation (using previous function)
    unsigned long startTime = millis();
    bool confirmed = false;
    while (millis() - startTime < 30000) { // 30s timeout
        auto statuses = connection.getSignatureStatuses({signature});
        if (!statuses.empty() && statuses[0].has_value() && 
            statuses[0]->confirmationStatus == "confirmed") {
            confirmed = true;
            break;
        }
        delay(1000);
    }
    TEST_ASSERT_TRUE(confirmed);
}

void setup() {
    delay(2000); // Allow serial monitor to start
    UNITY_BEGIN();
    RUN_TEST(test_anchor_transaction_completion);
    UNITY_END();
}

void loop() {}
