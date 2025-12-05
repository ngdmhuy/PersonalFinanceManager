//
//  Expense.cpp
//  PersonalFinanceManager
//
//  Created by Huy Nguyen on 4/12/25.
//

#include "../../include/Models/Expense.h"

Expense::Expense(std::string id, std::string walletId, std::string catId, double amount, Date date, std::string desc)
: Transaction(id, walletId, catId, amount, TransactionType::Expense, date, desc) { }
