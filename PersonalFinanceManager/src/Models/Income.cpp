//
//  Income.cpp
//  PersonalFinanceManager
//
//  Created by Nguyen Dinh Minh Huy on 4/12/25.
//

#include "../../include/Models/Income.h"

Income::Income(std::string id, std::string walletId, std::string catId, double amount, Date date, std::string desc)
    : Transaction(id, walletId, catId, amount, TransactionType::Income, date, desc) {
    // Type is automatically set to Income by the base constructor call
}
