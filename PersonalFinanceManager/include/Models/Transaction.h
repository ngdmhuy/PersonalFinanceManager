//
//  Transaction.h
//  PersonalFinanceManager
//
//  Created by Nguyen Dinh Minh Huy on 30/11/25.
//

#ifndef Transaction_h
#define Transaction_h

#include "../Utils/Date.h"
#include "../Utils/Enums.h"
#include "../Utils/BinaryFileHelper.h"

#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <sstream>

/**
 * @class Transaction
 * @brief Base class representing a single financial record.
 *
 * This abstract class holds common data shared by both Income and Expense,
 * such as amount, date, description, and the associated wallet.
 */
class Transaction {
protected:
    std::string id;
    std::string walletId;
    std::string categoryId; // Note: For Income, this might be SourceId
    double amount;
    std::string description;
    Date date;
    TransactionType type;
    
public:
    // ==========================================
    // 1. CONSTRUCTORS & DESTRUCTOR
    // ==========================================
    Transaction();
    
    /**
     * @brief Parameterized Constructor.
     * @param id Unique Transaction ID.
     * @param walletId The ID of the wallet this transaction affects.
     * @param catId Category ID (for Expense) or Source ID (for Income).
     * @param amount The monetary value.
     * @param type Enum indicating Income or Expense.
     * @param date Date of the transaction.
     * @param desc Short description.
     */
    Transaction(std::string id, std::string walletId, std::string catId, double amount, TransactionType type, Date date, std::string desc);
    
    virtual ~Transaction() = default;
    
    // ==========================================
    // 2. GETTERS (ACCESSORS)
    // ==========================================
    std::string GetId() const;
    std::string GetWalletId() const;
    std::string GetCategoryId() const;
    double GetAmount() const;
    Date GetDate() const;
    TransactionType GetType() const;
    std::string GetDescription() const;
    
    // ==========================================
    // 3. SETTERS (MUTATORS)
    // ==========================================
    void SetAmount(double a);
    void SetWalletId(const std::string& w);
    void SetCategoryId(const std::string& c);
    void SetDescription(const std::string& d);
    void SetDate(const Date& d);
    
    // ==========================================
    // 4. DISPLAY & SERIALIZATION
    // ==========================================
    /**
     * @brief Returns a string representation of the transaction.
     * Useful for debugging or console output.
     */
    virtual std::string ToString() const;
    
    // ==========================================
    // 5. SERIALIZATION
    // ==========================================
    
    /**
     * @brief Serializes the object to a binary stream.
     * Order: Type -> ID -> WalletID -> CategoryID -> amount -> date -> description
     */
    virtual void ToBinary(std::ofstream& fout) const;

    /**
     * @brief Factory method to create a Transaction from a binary stream.
     * Reads in the same order as ToBinary.
     */
    static Transaction* FromBinary(std::ifstream& fin);
};

#endif // !Transaction_h
