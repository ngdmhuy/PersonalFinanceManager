//
//  RecurringTransaction.cpp
//  PersonalFinanceManager
//
//  Created by Nguyen Dinh Minh Huy on 5/12/25.
//

#include "Models/Transaction.h"
#include "Models/Income.h"
#include "Models/Expense.h"
#include "Models/RecurringTransaction.h"
#include "Utils/BinaryFileHelper.h"
#include <sstream>
#include <iomanip>
#include <ctime>

// ==========================================
// 1. CONSTRUCTORS
// ==========================================

RecurringTransaction::RecurringTransaction()
    : amount(0.0), type(TransactionType::Expense), frequency(Frequency::Monthly) {
}

RecurringTransaction::RecurringTransaction(std::string id, Frequency freq, Date start, Date end, std::string walletId, std::string catId, double amount, TransactionType type, std::string desc)
    : id(id), frequency(freq), startDate(start), endDate(end), walletId(walletId), categoryID(catId), amount(amount), type(type), description(desc) {
}

RecurringTransaction::~RecurringTransaction() = default;

// ==========================================
// 2. GETTERS
// ==========================================

std::string RecurringTransaction::GetId() const { return id; }
Frequency RecurringTransaction::GetFrequency() const { return frequency; }
Date RecurringTransaction::GetStartDate() const { return startDate; }
Date RecurringTransaction::GetEndDate() const { return endDate; }
Date RecurringTransaction::GetLastGeneratedDate() const { return lastGeneratedDate; }

std::string RecurringTransaction::GetWalletId() const { return walletId; }
std::string RecurringTransaction::GetCategoryId() const { return categoryID; }
double RecurringTransaction::GetAmount() const { return amount; }
TransactionType RecurringTransaction::GetType() const { return type; }
std::string RecurringTransaction::GetDescription() const { return description; }

Date RecurringTransaction::GetNextDueDate() const {
    Date nextDueDate;
    if (!lastGeneratedDate.IsValid()) {
        nextDueDate = startDate;
    }
    else {
        switch (frequency) {
            case Frequency::Daily:
                nextDueDate = lastGeneratedDate.AddDays(1);
                break;
            case Frequency::Weekly:
                nextDueDate = lastGeneratedDate.AddWeeks(1);
                break;
            case Frequency::Monthly:
                nextDueDate = lastGeneratedDate.AddMonths(1);
                break;
            case Frequency::Yearly:
                nextDueDate = lastGeneratedDate.AddYears(1);
                break;
            default:
                break;
        }
    }
    
    return nextDueDate;
}

// ==========================================
// 3. SETTERS
// ==========================================

void RecurringTransaction::SetFrequency(Frequency f) { frequency = f; }
void RecurringTransaction::SetStartDate(const Date& d) { startDate = d; }
void RecurringTransaction::SetEndDate(const Date& d) { endDate = d; }
void RecurringTransaction::SetLastGeneratedDate(const Date& d) { lastGeneratedDate = d; }

void RecurringTransaction::SetWalletId(const std::string& w) { walletId = w; }
void RecurringTransaction::SetCategoryId(const std::string& c) { categoryID = c; }
void RecurringTransaction::SetAmount(double a) { amount = a; }
void RecurringTransaction::SetDescription(const std::string& d) { description = d; }

// ==========================================
// 4. AUTOMATION LOGIC
// ==========================================

Transaction* RecurringTransaction::GenerateTransaction(std::string newTransId, const Date& dateToCreate) {
    Transaction* t = nullptr;
    std::string recurringDesc = description + " (Auto)";
    
    if (type == TransactionType::Income)
        t = new Income(newTransId, walletId, categoryID, amount, dateToCreate, recurringDesc);
    else
        t = new Expense(newTransId, walletId, categoryID, amount, dateToCreate, recurringDesc);
    
    lastGeneratedDate = dateToCreate;
    return t;
}

bool RecurringTransaction::ShouldGenerate(const Date& currentDate) {
    // 1. Check End Date
    if (endDate.IsValid() && currentDate > endDate) return false;
    
    // 2. Check Start Date
    if (currentDate < startDate) return false;
    
    // 3. Check frequency logic
    Date nextDueDate = GetNextDueDate();
    return nextDueDate <= currentDate;
}

// ==========================================
// 5. DISPLAY
// ==========================================

std::string RecurringTransaction::ToString() const {
    std::stringstream ss;
    
    ss << "[" << EnumHelper::FrequencyToString(frequency) << "] Start: " << startDate.ToString();
    if (endDate.IsValid()) ss << " | End: " << endDate.ToString();
    else ss << " | Indefinite";
    
    if (lastGeneratedDate.IsValid()) ss << " | Last Generated: " << lastGeneratedDate.ToString();
    
    ss << (type == TransactionType::Income ? " + " : " - ")
       << std::fixed << std::setprecision(2) << amount
       << " | " << description;
    
    return ss.str();
}

// ==========================================
// 6. SERIALIZATION
// ==========================================

void RecurringTransaction::ToBinary(std::ofstream& fout) const {
    BinaryFileHelper::WriteString(fout, id);
    BinaryFileHelper::Write<int>(fout, static_cast<int>(frequency));
    BinaryFileHelper::WriteDate(fout, startDate);
    BinaryFileHelper::WriteDate(fout, endDate);
    BinaryFileHelper::WriteDate(fout, lastGeneratedDate);
    
    BinaryFileHelper::WriteString(fout, walletId);
    BinaryFileHelper::WriteString(fout, categoryID);
    BinaryFileHelper::Write<double>(fout, amount);
    BinaryFileHelper::Write<int>(fout, static_cast<int>(type));
    BinaryFileHelper::WriteString(fout, description);
}

RecurringTransaction* RecurringTransaction::FromBinary(std::ifstream& fin) {
    std::string id = BinaryFileHelper::ReadString(fin);
    Frequency freq = static_cast<Frequency>(BinaryFileHelper::Read<int>(fin));
    Date start = BinaryFileHelper::ReadDate(fin);
    Date end = BinaryFileHelper::ReadDate(fin);
    Date lastGen = BinaryFileHelper::ReadDate(fin);
    
    std::string wId = BinaryFileHelper::ReadString(fin);
    std::string catId = BinaryFileHelper::ReadString(fin);
    double amt = BinaryFileHelper::Read<double>(fin);
    TransactionType type = static_cast<TransactionType>(BinaryFileHelper::Read<int>(fin));
    std::string desc = BinaryFileHelper::ReadString(fin);
    
    RecurringTransaction* rt = new RecurringTransaction(id, freq, start, end, wId, catId, amt, type, desc);
    rt->SetLastGeneratedDate(lastGen);
    
    return rt;
}
