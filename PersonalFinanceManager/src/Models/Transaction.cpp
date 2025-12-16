//
//  Transaction.cpp
//  PersonalFinanceManager
//
//  Created by Nguyen Dinh Minh Huy on 30/11/25.
//

#include "../../include/Models/Transaction.h"
#include "../../include/Models/Income.h"
#include "../../include/Models/Expense.h"


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

// ==========================================
// 5. SERIALIZATION (CORE LOGIC)
// ==========================================

void Transaction::ToBinary(std::ofstream& fout) const {
    // 1. Write the Type Identifier FIRST (for polymorphism)
    BinaryFileHelper::WriteInt(fout, static_cast<int>(type));
    
    // 2. Write common fields
    BinaryFileHelper::WriteString(fout, id);
    BinaryFileHelper::WriteString(fout, walletId);
    BinaryFileHelper::WriteString(fout, categoryId);
    BinaryFileHelper::WriteDouble(fout, amount);
    BinaryFileHelper::WriteDate(fout, date);
    BinaryFileHelper::WriteString(fout, description);
}

Transaction* Transaction::FromBinary(std::ifstream& fin) {
    // 1. Read the Type Identifier
    int typeCode = BinaryFileHelper::ReadInt(fin);
    TransactionType type = static_cast<TransactionType>(typeCode);
    
    // 2. Read Common Fields
    std::string id = BinaryFileHelper::ReadString(fin);
    std::string wId = BinaryFileHelper::ReadString(fin);
    std::string catId = BinaryFileHelper::ReadString(fin);
    double amt = BinaryFileHelper::ReadDouble(fin);
    Date d = BinaryFileHelper::ReadDate(fin);
    std::string desc = BinaryFileHelper::ReadString(fin);
    
    // 3. Factory Logic: Decide which class to instantiate
    if (type == TransactionType::Income)
        return new Income(id, wId, catId, amt, d, desc);
    else return new Expense(id, wId, catId, amt, d, desc);
}
