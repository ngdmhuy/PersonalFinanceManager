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

/**
 * @class IdGenerator
 * @brief Generates high-entropy unique identifiers.
 */
class IdGenerator {
private:
    static char GetRandomChar();
    
    static const size_t BLOCK_NUMBER = 3;
    static const size_t CHARS_PER_BLOCK = 4;
    
public:
    /**
     * @brief Generates a formatted ID string.
     * Format: PREFIX-XXXX-XXXX-XXXX
     * @param prefix A string prefix (e.g., "TRX", "WAL").
     */
    static std::string GenerateId(const std::string& prefix);
};

#endif // !IdGenerator_h
