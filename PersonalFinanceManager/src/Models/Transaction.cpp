//
//  Transaction.cpp
//  PersonalFinanceManager
//
//  Created by Nguyen Dinh Minh Huy on 30/11/25.
//

#include "../../include/Models/Transaction.h"

// Constructor
Transaction::Transaction() : amount(0.0), type(TransactionType::Expense) { }
Transaction::Transaction(std::string id, std::string walletId, std::string catId, double amount, TransactionType type, Date date, std::string desc = "") : id(id), walletId(walletId), categoryId(catId), amount(amount), type(type), date(date), description(desc) { }

// Getters
std::string Transaction::getId() const { return id; }
std::string Transaction::getWalletId() const { return walletId; }
std::string Transaction::getCategoryId() const { return categoryId; }
double Transaction::getAmount() const { return amount; }
Date Transaction::getDate() const { return date; }
TransactionType Transaction::getType() const { return type; }
std::string Transaction::getDescription() const { return description; }

// Setters
void Transaction::setAmount(double a) { amount = a; }
void Transaction::setWalletId(const std::string& w) { walletId = w; }
void Transaction::setCategoryId(const std::string& c) { categoryId = c; }
void Transaction::setDescription(const std::string& d) { description = d; }
void Transaction::setDate(const Date& d) { date = d; }

std::string Transaction::ToString() const {
    std::stringstream ss;
    // Format: [YYYY-MM-DD] | +/- Amount | Description
    ss << date << " | "
    << (type == TransactionType::Income ? "+ " : "- ")
    << std::fixed << std::setprecision(2) << amount
    << " | " << description;
    return ss.str();
}
