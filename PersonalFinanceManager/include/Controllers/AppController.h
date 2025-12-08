//
//  AppController.h
//  PersonalFinanceManager
//
//  Created by Le Tran Hoang Long (M2) on 6/12/25.
//

#ifndef AppController_h
#define AppController_h

#include <string>
#include <iostream>

// Include Utils
#include "../Utils/ArrayList.h"
#include "../Utils/HashMap.h"
#include "../Utils/Date.h"
#include "../Utils/IdGenerator.h"
#include "../Utils/Enums.h"
#include "../Utils/BinaryFileHelper.h"

// Include Models
#include "../Models/Transaction.h"
#include "../Models/Income.h"
#include "../Models/Expense.h"
#include "../Models/Wallet.h"
#include "../Models/Category.h"
#include "../Models/IncomeSource.h"
#include "../Models/RecurringTransaction.h"

/**
 * @class AppController
 * @brief The central brain of the application.
 *
 * Manages the lifecycle of Wallets, Transactions, and Master Data (Categories/Sources).
 * Acts as the bridge between the UI (M3) and Data Persistence (M1).
 */
class AppController {
private:
    // --- Data Storage (The "Database") ---
    ArrayList<Transaction*>* transactions;
    ArrayList<RecurringTransaction*>* recurringTransactions;

    // --- Fast Lookups (Indices) ---
    HashMap<std::string, Wallet*>* walletsMap;
    HashMap<std::string, Category*>* categoriesMap;
    HashMap<std::string, IncomeSource*>* incomeSourcesMap;

    // --- Iteration Lists (For UI Menus) ---
    ArrayList<Wallet*>* walletsList;
    ArrayList<Category*>* categoriesList;
    ArrayList<IncomeSource*>* incomeSourcesList;

    /**
     * @brief Internal check to generate automatic transactions.
     * Called automatically during initialization.
     */
    void ProcessRecurringTransactions();

public:
    // ==========================================
    // 1. CONSTRUCTOR & DESTRUCTOR
    // ==========================================
    AppController();
    ~AppController();

    // ==========================================
    // 2. DATA PERSISTENCE
    // ==========================================
    /**
     * @brief Loads all data from binary files into memory.
     * Order: Categories/Sources -> Wallets -> Transactions.
     */
    void LoadData();

    /**
     * @brief Saves all current memory data to binary files.
     */
    void SaveData();

    // ==========================================
    // 3. WALLET LOGIC
    // ==========================================
    /**
     * @brief Creates a new wallet and adds it to both List and Map.
     * @param name Display name of the wallet.
     * @param initialBalance Starting amount.
     */
    void AddWallet(const std::string& name, double initialBalance);

    /**
     * @brief Finds a wallet by its unique ID.
     * @return Pointer to Wallet or nullptr if not found.
     */
    Wallet* GetWalletById(const std::string& id);

    /**
     * @brief Gets the list of all wallets (for display).
     */
    ArrayList<Wallet*>* GetWalletsList() const { return walletsList; }

    /**
     * @brief Calculates the sum of balances across all wallets.
     */
    double GetTotalBalance() const;

    // ==========================================
    // 4. MASTER DATA (CATEGORIES & SOURCES)
    // ==========================================
    void AddCategory(const std::string& name);
    Category* GetCategoryById(const std::string& id);
    ArrayList<Category*>* GetCategoriesList() const { return categoriesList; }

    void AddIncomeSource(const std::string& name);
    IncomeSource* GetIncomeSourceById(const std::string& id);
    ArrayList<IncomeSource*>* GetIncomeSourcesList() const { return incomeSourcesList; }

    // ==========================================
    // 5. TRANSACTION CORE LOGIC
    // ==========================================
    /**
     * @brief Creates a transaction and automatically updates the Wallet balance.
     * @param amount The value of the transaction (must be > 0).
     * @param walletId The ID of the wallet to charge/refund.
     * @param categoryOrSourceId Category ID (if Expense) or Source ID (if Income).
     * @param type Income or Expense.
     * @param date Date of transaction.
     * @param description Short text description.
     */
    void AddTransaction(double amount, std::string walletId, std::string categoryOrSourceId, TransactionType type, Date date, std::string description);
    
    /**
     * @brief Deletes a transaction and reverses the balance change on the Wallet.
     * @return true if successful, false if ID not found.
     */
    bool DeleteTransaction(const std::string& transactionId);

    ArrayList<Transaction*>* GetTransactions() const { return transactions; }
    
    // ==========================================
    // 6. AUTOMATION & REPORTING
    // ==========================================
    /**
     * @brief Schedules a recurring transaction to happen automatically in the future.
     */
    void AddRecurringTransaction(Frequency freq, Date startDate, Date endDate, std::string walletId, std::string categoryId, double amount, TransactionType type, std::string desc);

    /**
     * @brief Filters transactions within a specific date range.
     * Useful for generating reports or charts.
     */
    ArrayList<Transaction*>* GetTransactionsByDateRange(Date start, Date end);
};

#endif // !AppController_h
