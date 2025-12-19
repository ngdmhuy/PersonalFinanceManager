//
//  AppController.h
//  PersonalFinanceManager
//
//  Created by Le Tran Hoang Long (M2) on 6/12/25.
//

#ifndef AppController_h
#define AppController_h

#include <string>

#include "Utils/ArrayList.h"
#include "Utils/HashMap.h"
#include "Utils/Date.h"
#include "Utils/Enums.h"
#include "Views/ConsoleView.h"

class Transaction;
class RecurringTransaction;
class Wallet;
class Category;
class IncomeSource;

class AppController {
private:
    // --- UI Messaging ---
    ConsoleView* view;

    // --- Data Storage (The "Database") ---
    ArrayList<Transaction*>* transactions;
    ArrayList<RecurringTransaction*>* recurringTransactions;

    // --- Fast Lookups (Indices) ---
    HashMap<std::string, Wallet*>* walletsMap;
    HashMap<std::string, Category*>* categoriesMap;
    HashMap<std::string, IncomeSource*>* incomeSourcesMap;
    HashMap<std::string, Transaction*>* transactionsMap;
    HashMap<std::string, RecurringTransaction*>* recurringTransactionsMap;

    // --- Iteration Lists (For UI Menus) ---
    ArrayList<Wallet*>* walletsList;
    ArrayList<Category*>* categoriesList;
    ArrayList<IncomeSource*>* incomeSourcesList;

    // --- [OPTIMIZATION] Indexing (Cache transaction lists by ID) ---
    // Key: WalletID -> Value: List of Transactions in that wallet
    HashMap<std::string, ArrayList<Transaction*>*>* walletIndex;
    
    // Key: CategoryID -> Value: List of Transactions in that category
    HashMap<std::string, ArrayList<Transaction*>*>* categoryIndex;

    // Helper functions to keep indices in sync
    void AddTransactionToIndex(Transaction* t);
    void RemoveTransactionFromIndex(Transaction* t);
    
    /**
     * @brief Internal check to generate automatic transactions.
     * Called automatically during initialization.
     */
    void ProcessRecurringTransactions();
public:

    // 1. CONSTRUCTOR & DESTRUCTOR
    AppController(ConsoleView* v);
    ~AppController();

    // 2. DATA PERSISTENCE
    void SaveData();
    void LoadData();

    // 3. WALLET LOGIC
    void AddWallet(const std::string& name, double initialBalance);
    Wallet* GetWalletById(const std::string& id);
    ArrayList<Wallet*>* GetWalletsList() const { return walletsList; }
    double GetTotalBalance() const;
    
    // --- [NEW] WALLET CRUD ---
    void EditWallet(const std::string& id, const std::string& newName);
    bool DeleteWallet(const std::string& id);

    // 4. MASTER DATA (CATEGORIES & SOURCES)
    void AddCategory(const std::string& name);
    Category* GetCategoryById(const std::string& id);
    ArrayList<Category*>* GetCategoriesList() const { return categoriesList; }
    
    // --- [NEW] CATEGORY CRUD ---
    void EditCategory(const std::string& id, const std::string& newName);
    bool DeleteCategory(const std::string& id);

    void AddIncomeSource(const std::string& name);
    IncomeSource* GetIncomeSourceById(const std::string& id);
    ArrayList<IncomeSource*>* GetIncomeSourcesList() const { return incomeSourcesList; }
    
    // --- [NEW] INCOME SOURCE CRUD ---
    void EditIncomeSource(const std::string& id, const std::string& newName);
    bool DeleteIncomeSource(const std::string& id);

    // 5. TRANSACTION CORE LOGIC
    void AddTransaction(double amount, std::string walletId, std::string categoryOrSourceId, TransactionType type, Date date, std::string description);
    bool DeleteTransaction(const std::string& transactionId);
    
    // --- [NEW] TRANSACTION EDIT ---
    bool EditTransaction(const std::string& id, double newAmount, Date newDate, std::string newDesc);

    ArrayList<Transaction*>* GetTransactions() const { return transactions; }
    
    // 6. AUTOMATION & REPORTING
    void AddRecurringTransaction(Frequency freq, Date startDate, Date endDate, std::string walletId, std::string categoryId, double amount, TransactionType type, std::string desc);
    ArrayList<RecurringTransaction*>* GetRecurringList() const { return recurringTransactions; }
    RecurringTransaction* GetRecurringById(const std::string& id);
    bool DeleteRecurringTransaction(const std::string& id);
    void EditRecurringTransaction(const std::string& id, Frequency freq, Date startDate, Date endDate, std::string walletId, std::string categoryId, double amount, TransactionType type, std::string desc);

    // --- FILTERS & STATISTICS ---
    ArrayList<Transaction*>* GetTransactionsByDateRange(Date start, Date end);
    
    // --- [NEW] ADVANCED FILTERS (For Statistics Engine) ---
    ArrayList<Transaction*>* GetTransactionsByWallet(const std::string& walletId);
    ArrayList<Transaction*>* GetTransactionsByCategory(const std::string& categoryId);
    ArrayList<Transaction*>* SearchTransactions(const std::string& keyword);

}; 

#endif // !AppController_h
