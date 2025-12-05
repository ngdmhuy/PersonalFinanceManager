//
//  Wallet.h
//  PersonalFinanceManager
//
//  Created by Nguyen Dinh Minh Huy on 30/11/25.
//


#ifndef Wallet_h
#define Wallet_h

#include <string>
#include <iostream>
#include <sstream>
#include <iomanip>

class Wallet {
private:
    std::string id;
    std::string name;
    double balance;
    
public:
    // Constructor
    Wallet();
    Wallet(std::string id, std::string name, double initialBalance);
    
    // Destructor
    virtual ~Wallet();
    
    // Getters
    std::string getId() const;
    std::string getName() const;
    double getBalance() const;
    
    // Setters
    void setName(const std::string& n);
    void setBalance(double b);
    
    // Logic
    void addAmount(double amount);
    void subtractAmount(double amount);
    
    // Display
    std::string ToString() const;
};

#endif // !Wallet_h

