//
//  Income.h
//  PersonalFinanceManager
//
//  Created by Nguyen Dinh Minh Huy on 4/12/25.
//

#ifndef Income_h
#define Income_h

#include "Transaction.h"
#include <string>

/**
 * @class Income
 * @brief Represents money coming into a wallet.
 * Inherits from Transaction.
 */
class Income : public Transaction {
public:
    /**
     * @brief Constructor for Income.
     * Automatically sets TransactionType to Income.
     */
    Income(std::string id, std::string walletId, std::string catId, double amount, Date date, std::string desc);
    
    // Serialization: We use the Base Transaction::ToBinary and Transaction::FromBinary
};

#endif // !Income_h
