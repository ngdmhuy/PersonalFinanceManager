//
//  Enums.h
//  PersonalFinanceManager
//
//  Created by Huy Nguyen on 5/12/25.
//

#ifndef Enums_h
#define Enums_h

#include <string>

enum class TransactionType {
    Income,
    Expense
};

enum class Frequency {
    Daily,
    Weekly,
    Monthly,
    Yearly
};

enum class IdPrefix {
    Transaction,
    Wallet,
    Category,
    IncomeSource,
    Recurring
};

/**
 * @class EnumHelper
 * @brief Static helpers to convert Enums to/from Strings.
 * Useful for ID generation and UI display.
 */
class EnumHelper {
public:
    static std::string IdPrefixToString(IdPrefix prefix) {
        switch (prefix) {
            case IdPrefix::Transaction:     return "TRX";
            case IdPrefix::Wallet:          return "WAL";
            case IdPrefix::Category:        return "CAT";
            case IdPrefix::IncomeSource:    return "SRC";
            case IdPrefix::Recurring:       return "REC";
            default:                        return "UNK";
        }
    }
    
    static std::string TypeToString(TransactionType type) {
        return (type == TransactionType::Income) ? "Income" : "Expense";
    }
    
    static std::string FrequencyToString(Frequency freq) {
        switch (freq) {
            case Frequency::Daily:      return "Daily";
            case Frequency::Weekly:     return "Weekly";
            case Frequency::Monthly:    return "Monthly";
            case Frequency::Yearly:     return "Yearly";
            default:                    return "Unknown";
        }
    }
};

#endif // !Enums_h
