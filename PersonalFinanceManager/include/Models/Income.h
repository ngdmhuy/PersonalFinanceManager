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

class Income : public Transaction {
public:
    Income(std::string id, std::string walletId, std::string catId, double amount, Date date, std::string desc);
};

#endif /* Income_h */
