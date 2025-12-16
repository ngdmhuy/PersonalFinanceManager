//
//  Wallet.h
//  PersonalFinanceManager
//
//  Created by Nguyen Dinh Minh Huy on 30/11/25.
//

#ifndef Wallet_h
#define Wallet_h

#include "../Utils/BinaryFileHelper.h"

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>

/**
 * @class Wallet
 * @brief Represents a container for money (e.g., Cash, Bank Account).
 */
class Wallet {
private:
    std::string id;
    std::string name;
    double balance;
    
public:
    // ==========================================
    // 1. CONSTRUCTORS & DESTRUCTOR
    // ==========================================
    Wallet();
    Wallet(std::string id, std::string name, double initialBalance);
    ~Wallet();
    
    // ==========================================
    // 2. GETTERS & SETTERS
    // ==========================================
    std::string GetId() const;
    std::string GetName() const;
    double GetBalance() const;
    
    void SetName(const std::string& n);
    
    /**
     * @brief Manually sets the balance.
     * @warning Prefer using AddAmount/SubtractAmount for transactions.
     */
    void SetBalance(double b);
    
    // ==========================================
    // 3. BUSINESS LOGIC
    // ==========================================
    /**
     * @brief Increases wallet balance. Used for Income.
     */
    void AddAmount(double amount);

    /**
     * @brief Decreases wallet balance. Used for Expense.
     */
    void SubtractAmount(double amount);
    
    // ==========================================
    // 4. DISPLAY
    // ==========================================
    std::string ToString() const;

    // ==========================================
    // 5. SERIALIZATION
    // ==========================================
    
    /**
     * @brief Serializes the object to a binary stream.
     * Order: ID -> Name -> Balance
     */
    void ToBinary(std::ofstream& fout) const;

    /**
     * @brief Factory method to create a Wallet from a binary stream.
     * Reads in the same order as ToBinary.
     */
    static Wallet* FromBinary(std::ifstream& fin);
};

#endif // !Wallet_h
