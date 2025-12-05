//
//  Transaction.h
//  PersonalFinanceManager
//
//  Created by Nguyen Dinh Minh Huy on 30/11/25.
//


#ifndef Transaction_h
#define Transaction_h

#include "../Utils/Date.h"
#include "../Utils/Enums.h"

#include <string>
#include <iostream>
#include <iomanip>
#include <sstream>

class Transaction {
protected:
    std::string id;
    std::string walletId;
    std::string categoryId;
    double amount;
    std::string description;
    Date date;
    TransactionType type;
    
public:
    // Constructor
    Transaction();
    Transaction(std::string id, std::string walletId, std::string catId, double amount, TransactionType type, Date date, std::string desc);
    
    // Destructor
    virtual ~Transaction() = default;
    
    // Getters
    std::string getId() const;
    std::string getWalletId() const;
    std::string getCategoryId() const;
    double getAmount() const;
    Date getDate() const;
    TransactionType getType() const;
    std::string getDescription() const;
    
    // Setters
    void setAmount(double a);
    void setWalletId(const std::string& w);
    void setCategoryId(const std::string& c);
    void setDescription(const std::string& d);
    void setDate(const Date& d);
    
    // Virtual Display
    virtual std::string ToString() const;
};

#endif // !Transaction_h
