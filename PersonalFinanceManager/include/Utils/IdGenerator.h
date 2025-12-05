//
//  IdGenerator.h
//  PersonalFinanceManager
//
//  Created by Nguyen Dinh Minh Huy on 5/12/25.
//

#ifndef IdGenerator_h
#define IdGenerator_h

#include <string>
#include <random>
#include <sstream>

class IdGenerator {
private:
    /// Get random alphanumeric character
    static char GetRandomChar();
    
    static const size_t BLOCK_NUMBER = 3;
    static const size_t CHARS_PER_BLOCK = 4;
    
public:
    /// Generates a "High Entropy Alphanumeric ID" with an optional prefix to indicate whether Transaction, Wallet, Category,...
    /// e.g. Transaction: TRX-a9K2-mP5v-Qr8z
    static std::string GenerateId(const std::string& prefix);
};

#endif // !IdGenerator_h
