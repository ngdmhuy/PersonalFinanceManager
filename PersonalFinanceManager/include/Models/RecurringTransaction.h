//
//  RecurringTransaction.h
//  PersonalFinanceManager
//
//  Created by Nguyen Dinh Minh Huyn on 5/12/25.
//

#ifndef RecurringTransaction_h
#define RecurringTransaction_h

#include "Transaction.h"
#include "Income.h"
#include "Expense.h"
#include "../Utils/Date.h"
#include "../Utils/Enums.h"

#include <string>

class RecurringTransaction {
    std::string id;
    
    // Scheduling
    Frequency frequency;
    Date startDate;
    Date endDate;
    Date lastGeneratedDate;
    
    // Template Data
    std::string walletId;
    std::string categoryID;
    double amount;
    TransactionType type;
    std::string description;
    
public:
    // Constructors
    RecurringTransaction();
    RecurringTransaction(std::string id, Frequency freq, Date start, Date end, std::string walletId, std::string catId, double amount, TransactionType type, std::string desc);
    
    // Destructors
    ~RecurringTransaction();
    
    // Getters
    std::string getId() const;
    Frequency getFrequency() const;
    Date getStartDate() const;
    Date getEndDate() const;
    Date getLastGeneratedDate() const;
    
    std::string getWalletId() const;
    std::string getCategoryId() const;
    double getAmount() const;
    TransactionType getType() const;
    std::string getDescription() const;
    
    // Setters
    void setFrequency(Frequency f);
    void setStartDate(const Date& d);
    void setEndDate(const Date& d);
    void setLastGeneratedDate(const Date& d);
    
    void setWalletId(const std::string& walletId);
    void setCategoryId(const std::string& catId);
    void setAmount(double a);
    void setDescription(const std::string& d);
    
    // Logic
    Transaction* GenerateTransaction(std::string newTransId, const Date& dateToCreate);
    bool ShouldGenerate(const Date& currentDate);
    
    // Display
    std::string ToString() const;
};

#endif /* RecurringTransaction_h */
