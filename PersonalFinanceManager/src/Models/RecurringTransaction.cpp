//
//  RecurringTransaction.cpp
//  PersonalFinanceManager
//
//  Created by Nguyen Dinh Minh Huy on 5/12/25.
//

#include "../../include/Models/RecurringTransaction.h"

// Constructors
RecurringTransaction::RecurringTransaction() : amount(0.0), type(TransactionType::Expense), frequency(Frequency::Monthly) { }

RecurringTransaction::RecurringTransaction(std::string id, Frequency freq, Date start, Date end, std::string walletId, std::string catId, double amount, TransactionType type, std::string desc): id(id), frequency(freq), startDate(start), endDate(end), walletId(walletId), categoryID(catId), amount(amount), type(type), description(desc) { }

// Destructors
RecurringTransaction::~RecurringTransaction() = default;

// Getters
std::string RecurringTransaction::getId() const { return id; }
Frequency RecurringTransaction::getFrequency() const { return frequency; }
Date RecurringTransaction::getStartDate() const { return startDate; }
Date RecurringTransaction::getEndDate() const { return endDate; }
Date RecurringTransaction::getLastGeneratedDate() const { return lastGeneratedDate; }

std::string RecurringTransaction::getWalletId() const { return walletId; }
std::string RecurringTransaction::getCategoryId() const { return categoryID; }
double RecurringTransaction::getAmount() const { return amount; }
TransactionType RecurringTransaction::getType() const { return type; }
std::string RecurringTransaction::getDescription() const { return description; }

// Setters
void RecurringTransaction::setFrequency(Frequency f) { frequency = f; }
void RecurringTransaction::setStartDate(const Date& d) { startDate = d; }
void RecurringTransaction::setEndDate(const Date& d) { endDate = d; }
void RecurringTransaction::setLastGeneratedDate(const Date& d) { lastGeneratedDate = d; }

void RecurringTransaction::setWalletId(const std::string& w) { walletId = w; }
void RecurringTransaction::setCategoryId(const std::string& c) { categoryID = c; }
void RecurringTransaction::setAmount(double a) { amount = a; }
void RecurringTransaction::setDescription(const std::string& d) { description = d; }

// Logic
Transaction* RecurringTransaction::GenerateTransaction(std::string newTransId, const Date& dateToCreate) {
    Transaction* t = nullptr;
    std::string recurringDesc = description + " (Auto)";
    
    if (type == TransactionType::Income)
        t = new Income(newTransId, walletId, categoryID, amount, dateToCreate, recurringDesc);
    else t = new Expense(newTransId, walletId, categoryID, amount, dateToCreate, recurringDesc);
    
    lastGeneratedDate = dateToCreate;
    return t;
}

bool RecurringTransaction::ShouldGenerate(const Date& currentDate) {
    if (endDate.IsValid() && currentDate > endDate) return false;
    if (currentDate < startDate) return false;
    if(!lastGeneratedDate.IsValid()) return true;
    return lastGeneratedDate < currentDate;
}

// Display
std::string RecurringTransaction::ToString() const {
    std::stringstream ss;
    
    ss << "[" << EnumHelper::FrequencyToString(frequency) << "] Start: " << startDate;
    if (endDate.IsValid()) ss << " | End: " << endDate.ToString();
    else ss << " | Indefinite";
    
    ss << (type == TransactionType::Income ? " + " : " - ") << std::fixed << std::setprecision(2) << amount << " | " << description;
    
    return ss.str();
}
