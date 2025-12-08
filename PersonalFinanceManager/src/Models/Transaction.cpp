//
//  Transaction.cpp
//  PersonalFinanceManager
//
//  Created by Nguyen Dinh Minh Huy on 30/11/25.
//

#include "../../include/Models/Transaction.h"

// ==========================================
// 1. CONSTRUCTORS
// ==========================================

Transaction::Transaction()
    : amount(0.0), type(TransactionType::Expense) {
}

Transaction::Transaction(std::string id, std::string walletId, std::string catId, double amount, TransactionType type, Date date, std::string desc)
    : id(id), walletId(walletId), categoryId(catId),
      amount(amount), type(type), date(date), description(desc) {
}

// ==========================================
// 2. GETTERS
// ==========================================

std::string Transaction::GetId() const { return id; }
std::string Transaction::GetWalletId() const { return walletId; }
std::string Transaction::GetCategoryId() const { return categoryId; }
double Transaction::GetAmount() const { return amount; }
Date Transaction::GetDate() const { return date; }
TransactionType Transaction::GetType() const { return type; }
std::string Transaction::GetDescription() const { return description; }

// ==========================================
// 3. SETTERS
// ==========================================

void Transaction::SetAmount(double a) { amount = a; }
void Transaction::SetWalletId(const std::string& w) { walletId = w; }
void Transaction::SetCategoryId(const std::string& c) { categoryId = c; }
void Transaction::SetDescription(const std::string& d) { description = d; }
void Transaction::SetDate(const Date& d) { date = d; }

// ==========================================
// 4. DISPLAY
// ==========================================

std::string Transaction::ToString() const {
    std::stringstream ss;
    // Format: YYYY-MM-DD | +/- 00.00 | Description
    ss << date << " | "
       << (type == TransactionType::Income ? "+ " : "- ")
       << std::fixed << std::setprecision(2) << amount
       << " | " << description;
    return ss.str();
}
