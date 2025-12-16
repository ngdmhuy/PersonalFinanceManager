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
#include "../Utils/BinaryFileHelper.h"

#include <iostream>
#include <fstream>
#include <string>

/**
 * @class RecurringTransaction
 * @brief A template for generating transactions automatically over time.
 *
 * Stores the schedule (Frequency, Start/End Date) and the template data
 * (Amount, Wallet, Category) needed to create the actual transaction.
 */
class RecurringTransaction {
    std::string id;
    
    // Scheduling Configuration
    Frequency frequency;
    Date startDate;
    Date endDate;
    Date lastGeneratedDate;
    
    // Template Data for the Transaction
    std::string walletId;
    std::string categoryID;
    double amount;
    TransactionType type;
    std::string description;
    
public:
    // ==========================================
    // 1. CONSTRUCTORS & DESTRUCTOR
    // ==========================================
    RecurringTransaction();
    
    /**
     * @brief Creates a new recurring schedule.
     */
    RecurringTransaction(std::string id, Frequency freq, Date start, Date end, std::string walletId, std::string catId, double amount, TransactionType type, std::string desc);
    
    ~RecurringTransaction();
    
    // ==========================================
    // 2. GETTERS (ACCESSORS)
    // ==========================================
    std::string GetId() const;
    Frequency GetFrequency() const;
    Date GetStartDate() const;
    Date GetEndDate() const;
    
    /**
     * @brief Returns the date when the last transaction was successfully created.
     */
    Date GetLastGeneratedDate() const;
    
    std::string GetWalletId() const;
    std::string GetCategoryId() const;
    double GetAmount() const;
    TransactionType GetType() const;
    std::string GetDescription() const;
    
    // ==========================================
    // 3. SETTERS (MUTATORS)
    // ==========================================
    void SetFrequency(Frequency f);
    void SetStartDate(const Date& d);
    void SetEndDate(const Date& d);
    void SetLastGeneratedDate(const Date& d);
    
    void SetWalletId(const std::string& walletId);
    void SetCategoryId(const std::string& catId);
    void SetAmount(double a);
    void SetDescription(const std::string& d);
    
    // ==========================================
    // 4. AUTOMATION LOGIC
    // ==========================================
    
    /**
     * @brief Creates a new concrete Transaction object based on this template.
     * @param newTransId The ID for the new transaction.
     * @param dateToCreate The date for the new transaction.
     * @return Pointer to a new Income or Expense object (Caller owns memory).
     */
    Transaction* GenerateTransaction(std::string newTransId, const Date& dateToCreate);
    
    /**
     * @brief Checks if a new transaction needs to be generated for the given date.
     * Compares currentDate with lastGeneratedDate and the Frequency.
     */
    bool ShouldGenerate(const Date& currentDate);
    
    // ==========================================
    // 5. DISPLAY
    // ==========================================
    std::string ToString() const;

    // TODO [M1]: Add toBinary(ofstream&) and fromBinary(ifstream&) here
    // ==========================================
    // 6. SERIALIZATION
    // ==========================================
    
    /**
     * @brief Serializes the object to a binary stream.
     * Order: ID -> Frequency -> StartDate -> EndDate -> LastGenDate -> WalletID -> CategoryID -> Amount -> Type -> Description
     */
    void ToBinary(std::ofstream& fout) const;

    /**
     * @brief Factory method to create a RecurringTransaction from a binary stream.
     * Reads in the same order as ToBinary.
     */
    static RecurringTransaction* FromBinary(std::ifstream& fin);
};

#endif // !RecurringTransaction_h
