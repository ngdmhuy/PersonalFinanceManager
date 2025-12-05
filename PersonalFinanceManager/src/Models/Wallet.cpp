
//
//  Wallet.cpp
//  PersonalFinanceManager
//
//  Created by Nguyen Dinh Minh Huy on 30/11/25.
//

#include "../../include/Models/Wallet.h"

// Constructor
Wallet::Wallet() : balance(0.0) { }
Wallet::Wallet(std::string id, std::string name, double initialBalance) : id(id), name(name), balance(initialBalance) { }

// Destructor
Wallet::~Wallet() = default;

// Getters
std::string Wallet::getId() const { return id; }
std::string Wallet::getName() const { return name; }
double Wallet::getBalance() const { return balance; }

// Setters
void Wallet::setName(const std::string& n) { name = n; }
void Wallet::setBalance(double b) { balance = b; }

// Logic
void Wallet::addAmount(double amount) { balance += amount; }
void Wallet::subtractAmount(double amount) { balance -= amount; }

// Display
std::string Wallet::ToString() const {
    std::stringstream ss;
    ss << name << " (ID: " << id << ") - Balance: " << std::fixed << std::setprecision(2) << balance;
    return ss.str();
}
