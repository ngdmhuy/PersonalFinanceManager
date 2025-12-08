//
//  Wallet.cpp
//  PersonalFinanceManager
//
//  Created by Nguyen Dinh Minh Huy on 30/11/25.
//

#include "../../include/Models/Wallet.h"

// ==========================================
// 1. CONSTRUCTORS
// ==========================================

Wallet::Wallet() : balance(0.0) { }

Wallet::Wallet(std::string id, std::string name, double initialBalance)
    : id(id), name(name), balance(initialBalance) {
}

Wallet::~Wallet() = default;

// ==========================================
// 2. GETTERS & SETTERS
// ==========================================

std::string Wallet::GetId() const { return id; }
std::string Wallet::GetName() const { return name; }
double Wallet::GetBalance() const { return balance; }

void Wallet::SetName(const std::string& n) { name = n; }
void Wallet::SetBalance(double b) { balance = b; }

// ==========================================
// 3. BUSINESS LOGIC
// ==========================================

void Wallet::AddAmount(double amount) {
    balance += amount;
}

void Wallet::SubtractAmount(double amount) {
    balance -= amount;
}

// ==========================================
// 4. DISPLAY
// ==========================================

std::string Wallet::ToString() const {
    std::stringstream ss;
    ss << name << " (ID: " << id << ") - Balance: "
       << std::fixed << std::setprecision(2) << balance;
    return ss.str();
}
