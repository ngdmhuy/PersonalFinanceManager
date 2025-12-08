//
//  Expense.h
//  PersonalFinanceManager
//
//  Created by Nguyen Dinh Minh Huy on 4/12/25.
//

#ifndef Expense_h
#define Expense_h

#include "Transaction.h"
#include <string>

/**
 * @class Expense
 * @brief Represents money leaving a wallet.
 * Inherits from Transaction.
 */
class Expense : public Transaction {
public:
    /**
     * @brief Constructor for Expense.
     * Automatically sets TransactionType to Expense.
     */
    Expense(std::string id, std::string walletId, std::string catId, double amount, Date date, std::string desc);
};

#endif // !Expense_h
