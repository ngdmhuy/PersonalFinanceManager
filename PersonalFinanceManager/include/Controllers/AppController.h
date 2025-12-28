//
//  AppController.h
//  PersonalFinanceManager
//
//  Created by Le Tran Hoang Long (M2) on 6/12/25.
//

#ifndef AppController_h
#define AppController_h

#include <string>
#include <thread>
#include <mutex>
#include <atomic>

#include "Utils/ArrayList.h"
#include "Utils/HashMap.h"
#include "Utils/Date.h"
#include "Utils/Enums.h"
#include "Views/ConsoleView.h"

// Forward Declarations
class Transaction;
class RecurringTransaction;
class Wallet;
class Category;
class IncomeSource;

class AppController {
private:
    // --- THREADING ---
    std::recursive_mutex dataMutex;
    std::thread autoSaveThread;
    std::atomic<bool> stopAutoSave;

    void AutoSaveWorker();
    void ShowAutoSaveIndicator();
    
    // --- UI MESSAGING ---
    ConsoleView* view;

    // --- DATA STORAGE ---
    ArrayList<Transaction*>* transactions;
    ArrayList<RecurringTransaction*>* recurringTransactions;
    ArrayList<Wallet*>* walletsList;
    ArrayList<Category*>* categoriesList;
    ArrayList<IncomeSource*>* incomeSourcesList;

    // --- ID MAPS  ---
    HashMap<std::string, Wallet*>* walletsMap;
    HashMap<std::string, Category*>* categoriesMap;
    HashMap<std::string, IncomeSource*>* incomeSourcesMap;
    HashMap<std::string, Transaction*>* transactionsMap;
    HashMap<std::string, RecurringTransaction*>* recurringTransactionsMap;

    // --- FAST INDICES  ---
    // Key: ID -> Value: List of Transactions
    HashMap<std::string, ArrayList<Transaction*>*>* walletIndex;
    HashMap<std::string, ArrayList<Transaction*>*>* categoryIndex;
    HashMap<std::string, ArrayList<Transaction*>*>* incomeSourceIndex; // [MỚI] Index cho Income Source

    // --- HELPERS ---
    void AddTransactionToIndex(Transaction* t);
    void RemoveTransactionFromIndex(Transaction* t);

public:
    // 1. CONSTRUCTOR & DESTRUCTOR
    AppController(ConsoleView* v);
    ~AppController();

    // 2. DATA PERSISTENCE
    void SaveData(bool silent = false);
    void LoadData();
    void ClearDatabase();

    // 3. WALLET MANAGEMENT
    void AddWallet(const std::string& name, double initialBalance);
    Wallet* GetWalletById(const std::string& id);
    void EditWallet(const std::string& id, const std::string& newName);
    bool DeleteWallet(const std::string& id);
    
    ArrayList<Wallet*>* GetWalletsList() const { return walletsList; }
    double GetTotalBalance() const;

    // 4. CATEGORY MANAGEMENT
    void AddCategory(const std::string& name);
    Category* GetCategoryById(const std::string& id);
    void EditCategory(const std::string& id, const std::string& newName);
    bool DeleteCategory(const std::string& id);
    
    ArrayList<Category*>* GetCategoriesList() const { return categoriesList; }

    // 5. INCOME SOURCE MANAGEMENT
    void AddIncomeSource(const std::string& name);
    IncomeSource* GetIncomeSourceById(const std::string& id);
    void EditIncomeSource(const std::string& id, const std::string& newName);
    bool DeleteIncomeSource(const std::string& id);
    
    ArrayList<IncomeSource*>* GetIncomeSourcesList() const { return incomeSourcesList; }

    // 6. TRANSACTION CORE LOGIC
    void AddTransaction(double amount, std::string walletId, std::string categoryOrSourceId, TransactionType type, Date date, std::string description);
    bool DeleteTransaction(const std::string& transactionId);
    bool EditTransaction(const std::string& id, double newAmount, Date newDate, std::string newDesc);
    
    ArrayList<Transaction*>* GetTransactions() const { return transactions; }

    // 7. AUTOMATION (Recurring)
    void AddRecurringTransaction(Frequency freq, Date startDate, Date endDate, std::string walletId, std::string categoryId, double amount, TransactionType type, std::string desc);
    bool DeleteRecurringTransaction(const std::string& id);
    void EditRecurringTransaction(const std::string& id, Frequency freq, Date startDate, Date endDate, std::string walletId, std::string categoryId, double amount, TransactionType type, std::string desc);
    
    void ProcessRecurringTransactions(); // Đã chuyển sang Public
    ArrayList<RecurringTransaction*>* GetRecurringList() const { return recurringTransactions; }
    RecurringTransaction* GetRecurringById(const std::string& id);

    // 8. FILTERS & STATISTICS
    ArrayList<Transaction*>* GetTransactionsByDateRange(Date start, Date end);
    ArrayList<Transaction*>* GetTransactionsByType(TransactionType type);
    
    // --- ADVANCED FILTERS ---
    ArrayList<Transaction*>* GetTransactionsByAmountRange(double minAmount, double maxAmount);
    ArrayList<Transaction*>* GetTransactionsByWallet(const std::string& walletId);
    ArrayList<Transaction*>* GetTransactionsByCategory(const std::string& categoryId);
    

    ArrayList<Transaction*>* GetTransactionsByIncomeSource(const std::string& sourceId);
    
    ArrayList<Transaction*>* SearchTransactions(const std::string& keyword);
};

#endif 