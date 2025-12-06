//
//  AppController.cpp
//  PersonalFinanceManager
//
//  Created by Le Tran Hoang Long (M2) on 6/12/25.
//  Implementation of Business Logic with strict Validation and Error Handling.
//

#include "../../include/Controllers/AppController.h"
#include <iostream>
#include <algorithm> // For checking whitespace

// =================================================================================
// INTERNAL HELPER: STRING VALIDATION
// =================================================================================

// Helper to check if a string is empty or contains only whitespace
// Addresses Case: "Tên ví rỗng hoặc toàn dấu cách"
bool isStringEmptyOrWhitespace(const std::string& str) {
    return str.find_first_not_of(' ') == std::string::npos;
}

// =================================================================================
// PART 1: CONSTRUCTOR, DESTRUCTOR & MASTER DATA (WALLET/CATEGORY)
// =================================================================================

AppController::AppController() {
    // 1. Initialize In-Memory Database Structures
    // Lists are used for sequential access (UI Menus, Reports)
    this->transactions = new ArrayList<Transaction*>();
    this->recurringTransactions = new ArrayList<RecurringTransaction*>();
    this->walletsList = new ArrayList<Wallet*>();
    this->categoriesList = new ArrayList<Category*>();
    this->incomeSourcesList = new ArrayList<IncomeSource*>();

    // Maps are used for O(1) random access (Logic Checks, ID Lookups)
    this->walletsMap = new HashMap<std::string, Wallet*>();
    this->categoriesMap = new HashMap<std::string, Category*>();
    this->incomeSourcesMap = new HashMap<std::string, IncomeSource*>();

    // 2. Load Data from Persistence Layer (Binary Files)
    // If files don't exist (First run or User deleted files), this should safely return without crashing.
    loadData();

    // 3. Automation Check on Startup
    // Addresses Case: "Ngủ đông" (User hasn't opened app in months)
    processRecurringTransactions();
}

AppController::~AppController() {
    // 1. Auto-save State (Persistence)
    // Addresses Case: "Mất điện/Crash" - We try to save on normal exit.
    // Ideally, save logic should write to temp file first, but here we call the standard save.
    saveData();

    // 2. CRITICAL MEMORY CLEANUP
    // We must delete the objects pointed to by the lists BEFORE deleting the containers.
    // Note: We only delete objects via the List. The Map holds pointers to the SAME objects,
    // so we don't delete objects again in the Map (Double Free Error).

    // Cleanup Transactions
    if (transactions) {
        for (size_t i = 0; i < transactions->Count(); ++i) delete transactions->Get(i);
        delete transactions;
    }

    // Cleanup Recurring Transactions
    if (recurringTransactions) {
        for (size_t i = 0; i < recurringTransactions->Count(); ++i) delete recurringTransactions->Get(i);
        delete recurringTransactions;
    }

    // Cleanup Wallets
    if (walletsList) {
        for (size_t i = 0; i < walletsList->Count(); ++i) delete walletsList->Get(i);
        delete walletsList;
    }
    if (walletsMap) delete walletsMap; // Only delete the container, contents already deleted via list

    // Cleanup Categories
    if (categoriesList) {
        for (size_t i = 0; i < categoriesList->Count(); ++i) delete categoriesList->Get(i);
        delete categoriesList;
    }
    if (categoriesMap) delete categoriesMap;

    // Cleanup Income Sources
    if (incomeSourcesList) {
        for (size_t i = 0; i < incomeSourcesList->Count(); ++i) delete incomeSourcesList->Get(i);
        delete incomeSourcesList;
    }
    if (incomeSourcesMap) delete incomeSourcesMap;
    
    std::cout << "[System] Memory cleaned up successfully.\n";
}

// --- WALLET LOGIC ---

void AppController::addWallet(const std::string& name, double initialBalance) {
    // [Edge Case] Empty name or only spaces
    if (isStringEmptyOrWhitespace(name)) {
        std::cout << "[Error] Wallet creation failed: Name cannot be empty or whitespace.\n";
        return;
    }
    
    // [Edge Case] Duplicate Name
    // Although ID distinguishes them, duplicate names confuse users.
    // We iterate the list to check.
    for (size_t i = 0; i < walletsList->Count(); ++i) {
        if (walletsList->Get(i)->getName() == name) {
            std::cout << "[Warning] A wallet with name '" << name << "' already exists. Creating duplicate...\n";
            // We allow it as per requirements, but logging a warning is good practice.
            break; 
        }
    }

    // [Happy Path] Create Wallet
    std::string prefix = EnumHelper::IdPrefixToString(IdPrefix::Wallet);
    std::string newId = IdGenerator::GenerateId(prefix);

    // Note: initialBalance can be negative (Credit Card) or 0. This is allowed.
    Wallet* newWallet = new Wallet(newId, name, initialBalance);

    // Sync both Data Structures
    walletsMap->Put(newId, newWallet);
    walletsList->Add(newWallet);
    
    std::cout << "[Success] Wallet created: " << name << " (ID: " << newId << ")\n";
}

Wallet* AppController::getWalletById(const std::string& id) {
    // Retrieve safe pointer from Map
    Wallet** w = walletsMap->Get(id);
    return (w != nullptr) ? *w : nullptr;
}

double AppController::getTotalBalance() const {
    double total = 0;
    // Addresses Case: "Ví Âm phủ" - Negative balances are correctly summed up
    for (size_t i = 0; i < walletsList->Count(); ++i) {
        total += walletsList->Get(i)->getBalance();
    }
    return total;
}

// --- MASTER DATA LOGIC (CATEGORY & INCOME SOURCE) ---

void AppController::addCategory(const std::string& name) {
    // [Edge Case] Validation
    if (isStringEmptyOrWhitespace(name)) {
        std::cout << "[Error] Category name cannot be empty.\n";
        return;
    }

    std::string prefix = EnumHelper::IdPrefixToString(IdPrefix::Category);
    std::string id = IdGenerator::GenerateId(prefix);
    
    Category* obj = new Category(id, name);
    categoriesMap->Put(id, obj);
    categoriesList->Add(obj);
}

Category* AppController::getCategoryById(const std::string& id) {
    Category** c = categoriesMap->Get(id);
    return (c != nullptr) ? *c : nullptr;
}

void AppController::addIncomeSource(const std::string& name) {
    // [Edge Case] Validation
    if (isStringEmptyOrWhitespace(name)) {
        std::cout << "[Error] Source name cannot be empty.\n";
        return;
    }
    
    std::string prefix = EnumHelper::IdPrefixToString(IdPrefix::IncomeSource);
    std::string id = IdGenerator::GenerateId(prefix);
    
    IncomeSource* obj = new IncomeSource(id, name);
    incomeSourcesMap->Put(id, obj);
    incomeSourcesList->Add(obj);
}

IncomeSource* AppController::getIncomeSourceById(const std::string& id) {
    IncomeSource** s = incomeSourcesMap->Get(id);
    return (s != nullptr) ? *s : nullptr;
}

// =================================================================================
// PART 2: TRANSACTION BUSINESS LOGIC (CRUD & BALANCE ROLLBACK)
// =================================================================================

void AppController::addTransaction(double amount, std::string walletId, std::string categoryOrSourceId, 
                                   TransactionType type, Date date, std::string description) {
    // [Guard] Prevent negative or zero amounts (Scenario: Negative or Zero Input)
    if (amount <= 0) {
        std::cout << "[Error] Transaction amount must be positive (>0).\n";
        return;
    }

    // [Guard] Ensure Wallet exists (Scenario: Dangling/Ghost Reference)
    Wallet* wallet = getWalletById(walletId);
    if (wallet == nullptr) {
        std::cout << "[Error] Wallet ID not found: " << walletId << "\n";
        return;
    }

    // [Guard] Validate Category or Source existence based on type
    if (type == TransactionType::Expense) {
        if (getCategoryById(categoryOrSourceId) == nullptr) {
             std::cout << "[Error] Expense Category not found.\n";
             return;
        }
    } else {
        // Assuming TransactionType::Income
        if (getIncomeSourceById(categoryOrSourceId) == nullptr) {
            std::cout << "[Error] Income Source not found.\n";
            return;
        }
    }

    // Generate ID: TRX-xxxx
    std::string prefix = EnumHelper::IdPrefixToString(IdPrefix::Transaction);
    std::string transId = IdGenerator::GenerateId(prefix);

    Transaction* newTrans = nullptr;

    // Apply Logic: Create Object AND Update Wallet Balance
    if (type == TransactionType::Income) {
        // Income Logic: Create Income object -> Add money to Wallet
        newTrans = new Income(transId, walletId, categoryOrSourceId, amount, date, description);
        wallet->addAmount(amount); 
    } else {
        // Expense Logic: Create Expense object -> Subtract money from Wallet
        // Note: Wallet logic allows negative balance (Overdraft) as discussed.
        newTrans = new Expense(transId, walletId, categoryOrSourceId, amount, date, description);
        wallet->subtractAmount(amount); 
    }

    transactions->Add(newTrans);
    std::cout << "[Success] Transaction added. New Wallet Balance: " << std::fixed << std::setprecision(0) << wallet->getBalance() << "\n";
}

// [CRITICAL FEATURE] Delete Transaction and Rollback Wallet Balance
// Addresses Case: "Delete Transaction" - Must restore wallet balance correctly.
bool AppController::deleteTransaction(const std::string& transactionId) {
    int foundIndex = -1;
    Transaction* target = nullptr;

    // 1. Find the transaction in the list
    for (size_t i = 0; i < transactions->Count(); ++i) {
        if (transactions->Get(i)->getId() == transactionId) {
            foundIndex = (int)i;
            target = transactions->Get(i);
            break;
        }
    }

    if (foundIndex == -1 || target == nullptr) {
        std::cout << "[Error] Transaction ID not found: " << transactionId << "\n";
        return false;
    }

    // 2. Rollback Balance (Reverse the operation)
    Wallet* w = getWalletById(target->getWalletId());
    
    // Check if wallet still exists (it might have been deleted manually by user)
    if (w != nullptr) {
        if (target->getType() == TransactionType::Income) {
            // If we delete an Income, we must SUBTRACT the money back from wallet
            w->subtractAmount(target->getAmount());
        } else {
            // If we delete an Expense, we must ADD the money back to wallet (Refund)
            w->addAmount(target->getAmount());
        }
        std::cout << "[Info] Wallet balance restored. New Balance: " << std::fixed << std::setprecision(0) << w->getBalance() << "\n";
    } else {
        std::cout << "[Warning] Linked Wallet not found. Transaction deleted but balance could not be restored.\n";
    }

    // 3. Remove from memory
    transactions->RemoveAt(foundIndex);
    
    // CRITICAL: Delete the object to prevent Memory Leak
    delete target; 

    return true;
}

// =================================================================================
// PART 3: RECURRING TRANSACTIONS (AUTOMATION) & REPORTING
// =================================================================================

void AppController::addRecurringTransaction(Frequency freq, Date startDate, Date endDate,
                                            std::string walletId, std::string categoryId,
                                            double amount, TransactionType type, std::string desc) {
    
    // [Guard] Date Validation: Start Date cannot be after End Date
    // Note: Assuming Date::operator> works correctly.
    // If endDate is empty (infinite), IsValid() should handle it or check manually.
    if (endDate.IsValid() && startDate > endDate) {
        std::cout << "[Error] Invalid Date Range: Start Date cannot be after End Date.\n";
        return;
    }
    
    // [Guard] Wallet Validation: Cannot schedule for a non-existent wallet
    if (getWalletById(walletId) == nullptr) {
        std::cout << "[Error] Wallet ID not found. Cannot schedule recurring transaction.\n";
        return;
    }

    // [Guard] Amount Validation
    if (amount <= 0) {
        std::cout << "[Error] Amount must be positive.\n";
        return;
    }

    // Generate ID for the Recurring Setup (REC-xxxx)
    std::string prefix = EnumHelper::IdPrefixToString(IdPrefix::Recurring);
    std::string id = IdGenerator::GenerateId(prefix);
    
    // Create and Store
    RecurringTransaction* rt = new RecurringTransaction(id, freq, startDate, endDate, 
                                                        walletId, categoryId, amount, type, desc);
    recurringTransactions->Add(rt);
    std::cout << "[Success] Recurring transaction scheduled successfully.\n";
}

void AppController::processRecurringTransactions() {
    Date today = Date::GetTodayDate();
    int generatedCount = 0;

    std::cout << "[System] Checking for recurring transactions...\n";

    for (size_t i = 0; i < recurringTransactions->Count(); ++i) {
        RecurringTransaction* rt = recurringTransactions->Get(i);

        // Check Logic: Is it due today? Has it run this month?
        if (rt->ShouldGenerate(today)) {
            
            // 1. Verify Wallet Existence (Edge Case: User might have deleted the wallet manually)
            Wallet* w = getWalletById(rt->getWalletId());
            if (w == nullptr) {
                std::cout << "[Warning] Skipping recurring ID " << rt->getId() << ": Wallet not found.\n";
                continue; 
            }

            // 2. Generate New Transaction ID
            std::string prefix = EnumHelper::IdPrefixToString(IdPrefix::Transaction);
            std::string newTransId = IdGenerator::GenerateId(prefix);
            
            // 3. Factory Method: Create actual Transaction from Template
            Transaction* autoTrans = rt->GenerateTransaction(newTransId, today);
            
            // 4. Update System State
            transactions->Add(autoTrans);
            
            // Update Wallet Balance
            if (rt->getType() == TransactionType::Income) {
                w->addAmount(rt->getAmount());
            } else {
                w->subtractAmount(rt->getAmount());
            }

            // 5. Update Tracker to prevent double-generation
            rt->setLastGeneratedDate(today);
            
            generatedCount++;
            std::cout << "[Auto] Generated: " << rt->getDescription() << " (" << rt->getAmount() << ")\n";
        }
    }

    if (generatedCount == 0) {
        std::cout << "[System] No new recurring transactions due today.\n";
    }
}

// =================================================================================
// PART 4: REPORTING HELPERS (FOR UI/M3)
// =================================================================================

// Helper to filter transactions for Reports/Charts
// Caller (M3) is responsible for deleting the returned ArrayList pointer
ArrayList<Transaction*>* AppController::getTransactionsByDateRange(Date start, Date end) {
    ArrayList<Transaction*>* result = new ArrayList<Transaction*>();

    for (size_t i = 0; i < transactions->Count(); ++i) {
        Transaction* t = transactions->Get(i);
        
        // Filter Logic: Inclusive Range [start, end]
        if (t->getDate() >= start && t->getDate() <= end) {
            result->Add(t);
        }
    }
    return result; 
}

// =================================================================================
// PART 5: DATA PERSISTENCE STUBS
// =================================================================================
// Note: These functions will integrate with M1's BinaryFileHelper later.

void AppController::loadData() {
    // TODO [M1]: Call BinaryFileHelper::LoadAll(wallets, categories, transactions...)
    // std::cout << "[System] Data loaded from disk.\n";
}

void AppController::saveData() {
    // TODO [M1]: Call BinaryFileHelper::SaveAll(wallets, categories, transactions...)
    // std::cout << "[System] Data saved to disk.\n";
}