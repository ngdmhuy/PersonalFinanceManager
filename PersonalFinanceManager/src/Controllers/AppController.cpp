//
//  AppController.cpp
//  PersonalFinanceManager
//
//  Created by Le Tran Hoang Long (M2) on 6/12/25.
//  Implementation of Business Logic with strict Validation and Error Handling.
//

#include "../../include/Controllers/AppController.h"
#include <iostream>
#include <iomanip>
#include <algorithm>

// ==========================================
// STATIC HELPER FUNCTIONS
// ==========================================

// Checks if a string is null, empty, or whitespace only
static bool IsStringEmptyOrWhitespace(const std::string& str) {
    return str.find_first_not_of(' ') == std::string::npos;
}

// ==========================================
// 1. CONSTRUCTOR & DESTRUCTOR
// ==========================================

AppController::AppController() {
    // 1. Initialize the Storage Lists (The "Tables")
    this->transactions = new ArrayList<Transaction*>();
    this->recurringTransactions = new ArrayList<RecurringTransaction*>();
    this->walletsList = new ArrayList<Wallet*>();
    this->categoriesList = new ArrayList<Category*>();
    this->incomeSourcesList = new ArrayList<IncomeSource*>();

    // 2. Initialize the HashMaps (The "Indices" for fast lookup)
    this->walletsMap = new HashMap<std::string, Wallet*>();
    this->categoriesMap = new HashMap<std::string, Category*>();
    this->incomeSourcesMap = new HashMap<std::string, IncomeSource*>();

    // 3. Load existing data from binary files
    LoadData();

    // 4. Run automation checks (Generate recurring items if due)
    ProcessRecurringTransactions();
}

AppController::~AppController() {
    // 1. Save state before closing
    SaveData();

    // 2. Clean up Transactions
    if (transactions) {
        // Delete the actual objects first
        for (size_t i = 0; i < transactions->Count(); ++i) delete transactions->Get(i);
        delete transactions; // Then delete the list container
    }

    // 3. Clean up Recurring Transactions
    if (recurringTransactions) {
        for (size_t i = 0; i < recurringTransactions->Count(); ++i) delete recurringTransactions->Get(i);
        delete recurringTransactions;
    }

    // 4. Clean up Wallets
    if (walletsList) {
        for (size_t i = 0; i < walletsList->Count(); ++i) delete walletsList->Get(i);
        delete walletsList;
    }
    // Only delete the Map container, not the contents (already deleted via list)
    if (walletsMap) delete walletsMap;

    // 5. Clean up Categories
    if (categoriesList) {
        for (size_t i = 0; i < categoriesList->Count(); ++i) delete categoriesList->Get(i);
        delete categoriesList;
    }
    if (categoriesMap) delete categoriesMap;

    // 6. Clean up Income Sources
    if (incomeSourcesList) {
        for (size_t i = 0; i < incomeSourcesList->Count(); ++i) delete incomeSourcesList->Get(i);
        delete incomeSourcesList;
    }
    if (incomeSourcesMap) delete incomeSourcesMap;
    
    std::cout << "[System] Memory cleaned up successfully.\n";
}

// ==========================================
// 2. DATA PERSISTENCE
// ==========================================

void AppController::LoadData() {
    // TODO [M1]: Implement loading logic using BinaryFileHelper
    // Order: Categories -> Wallets -> Transactions
    // std::cout << "[System] Data loaded from disk.\n";
}

void AppController::SaveData() {
    // TODO [M1]: Implement saving logic using BinaryFileHelper
    // std::cout << "[System] Data saved to disk.\n";
}

// ==========================================
// 3. WALLET LOGIC
// ==========================================

void AppController::AddWallet(const std::string& name, double initialBalance) {
    // Validation
    if (IsStringEmptyOrWhitespace(name)) {
        std::cout << "[Error] Wallet creation failed: Name cannot be empty.\n";
        return;
    }
    
    // Check for duplicate names (Business Rule)
    for (size_t i = 0; i < walletsList->Count(); ++i) {
        if (walletsList->Get(i)->GetName() == name) {
            std::cout << "[Warning] Wallet '" << name << "' already exists.\n";
            break;
        }
    }

    // ID Generation
    std::string prefix = EnumHelper::IdPrefixToString(IdPrefix::Wallet);
    std::string newId = IdGenerator::GenerateId(prefix);

    // Create Object
    Wallet* newWallet = new Wallet(newId, name, initialBalance);

    // Store in both List (for iteration) and Map (for lookup)
    walletsMap->Put(newId, newWallet);
    walletsList->Add(newWallet);
    
    std::cout << "[Success] Wallet created: " << name << " (ID: " << newId << ")\n";
}

Wallet* AppController::GetWalletById(const std::string& id) {
    Wallet** w = walletsMap->Get(id);
    return (w != nullptr) ? *w : nullptr;
}

double AppController::GetTotalBalance() const {
    double total = 0;
    for (size_t i = 0; i < walletsList->Count(); ++i) {
        total += walletsList->Get(i)->GetBalance();
    }
    return total;
}

// ==========================================
// 4. MASTER DATA (CATEGORIES & SOURCES)
// ==========================================

void AppController::AddCategory(const std::string& name) {
    if (IsStringEmptyOrWhitespace(name)) {
        std::cout << "[Error] Category name cannot be empty.\n";
        return;
    }

    std::string prefix = EnumHelper::IdPrefixToString(IdPrefix::Category);
    std::string id = IdGenerator::GenerateId(prefix);
    
    Category* obj = new Category(id, name);
    categoriesMap->Put(id, obj);
    categoriesList->Add(obj);
}

Category* AppController::GetCategoryById(const std::string& id) {
    Category** c = categoriesMap->Get(id);
    return (c != nullptr) ? *c : nullptr;
}

void AppController::AddIncomeSource(const std::string& name) {
    if (IsStringEmptyOrWhitespace(name)) {
        std::cout << "[Error] Source name cannot be empty.\n";
        return;
    }
    
    std::string prefix = EnumHelper::IdPrefixToString(IdPrefix::IncomeSource);
    std::string id = IdGenerator::GenerateId(prefix);
    
    IncomeSource* obj = new IncomeSource(id, name);
    incomeSourcesMap->Put(id, obj);
    incomeSourcesList->Add(obj);
}

IncomeSource* AppController::GetIncomeSourceById(const std::string& id) {
    IncomeSource** s = incomeSourcesMap->Get(id);
    return (s != nullptr) ? *s : nullptr;
}

// ==========================================
// 5. TRANSACTION CORE LOGIC
// ==========================================

void AppController::AddTransaction(double amount, std::string walletId, std::string categoryOrSourceId, TransactionType type, Date date, std::string description) {
    // 1. Validate Amount
    if (amount <= 0) {
        std::cout << "[Error] Transaction amount must be positive.\n";
        return;
    }

    // 2. Validate Wallet
    Wallet* wallet = GetWalletById(walletId);
    if (wallet == nullptr) {
        std::cout << "[Error] Wallet ID not found: " << walletId << "\n";
        return;
    }

    // 3. Validate Category or Source
    if (type == TransactionType::Expense) {
        if (GetCategoryById(categoryOrSourceId) == nullptr) {
             std::cout << "[Error] Expense Category not found.\n";
             return;
        }
    } else {
        if (GetIncomeSourceById(categoryOrSourceId) == nullptr) {
            std::cout << "[Error] Income Source not found.\n";
            return;
        }
    }

    // 4. Create Transaction
    std::string prefix = EnumHelper::IdPrefixToString(IdPrefix::Transaction);
    std::string transId = IdGenerator::GenerateId(prefix);

    Transaction* newTrans = nullptr;

    if (type == TransactionType::Income) {
        newTrans = new Income(transId, walletId, categoryOrSourceId, amount, date, description);
        wallet->AddAmount(amount); // Automatically increase balance
    } else {
        newTrans = new Expense(transId, walletId, categoryOrSourceId, amount, date, description);
        wallet->SubtractAmount(amount); // Automatically decrease balance
    }

    transactions->Add(newTrans);
    
    std::cout << "[Success] Transaction added. New Wallet Balance: "
              << std::fixed << std::setprecision(0) << wallet->GetBalance() << "\n";
}

bool AppController::DeleteTransaction(const std::string& transactionId) {
    int foundIndex = -1;
    Transaction* target = nullptr;

    // 1. Search for the transaction
    for (size_t i = 0; i < transactions->Count(); ++i) {
        if (transactions->Get(i)->GetId() == transactionId) {
            foundIndex = (int)i;
            target = transactions->Get(i);
            break;
        }
    }

    if (foundIndex == -1 || target == nullptr) {
        std::cout << "[Error] Transaction ID not found: " << transactionId << "\n";
        return false;
    }

    // 2. Restore the Wallet Balance (Undo logic)
    Wallet* w = GetWalletById(target->GetWalletId());
    if (w != nullptr) {
        if (target->GetType() == TransactionType::Income) {
            w->SubtractAmount(target->GetAmount()); // Undo Income -> Subtract
        } else {
            w->AddAmount(target->GetAmount());      // Undo Expense -> Add
        }
        std::cout << "[Info] Wallet balance restored. New Balance: "
                  << std::fixed << std::setprecision(0) << w->GetBalance() << "\n";
    } else {
        std::cout << "[Warning] Linked Wallet not found. Balance not restored.\n";
    }

    // 3. Remove and Delete
    transactions->RemoveAt(foundIndex);
    delete target; // Important: Free memory to avoid leaks
    return true;
}

// ==========================================
// 6. AUTOMATION & REPORTING
// ==========================================

void AppController::AddRecurringTransaction(Frequency freq, Date startDate, Date endDate, std::string walletId, std::string categoryId, double amount, TransactionType type, std::string desc) {
    // Validation
    if (endDate.IsValid() && startDate > endDate) {
        std::cout << "[Error] Invalid Date Range: Start > End.\n";
        return;
    }
    
    if (GetWalletById(walletId) == nullptr) {
        std::cout << "[Error] Wallet ID not found.\n";
        return;
    }

    if (amount <= 0) {
        std::cout << "[Error] Amount must be positive.\n";
        return;
    }

    std::string prefix = EnumHelper::IdPrefixToString(IdPrefix::Recurring);
    std::string id = IdGenerator::GenerateId(prefix);
    
    // Create and Store
    RecurringTransaction* rt = new RecurringTransaction(id, freq, startDate, endDate, walletId, categoryId, amount, type, desc);
    recurringTransactions->Add(rt);
    std::cout << "[Success] Recurring transaction scheduled.\n";
}

void AppController::ProcessRecurringTransactions() {
    Date today = Date::GetTodayDate();
    int generatedCount = 0;

    std::cout << "[System] Checking recurring transactions...\n";

    for (size_t i = 0; i < recurringTransactions->Count(); ++i) {
        RecurringTransaction* rt = recurringTransactions->Get(i);

        // Ask the recurring object if it's time to run
        if (rt->ShouldGenerate(today)) {
            Wallet* w = GetWalletById(rt->GetWalletId());
            if (w == nullptr) continue;

            // Generate a real transaction ID
            std::string prefix = EnumHelper::IdPrefixToString(IdPrefix::Transaction);
            std::string newTransId = IdGenerator::GenerateId(prefix);
            
            // Create the real transaction
            Transaction* autoTrans = rt->GenerateTransaction(newTransId, today);
            transactions->Add(autoTrans);
            
            // Update wallet balance immediately
            if (rt->GetType() == TransactionType::Income) {
                w->AddAmount(rt->GetAmount());
            } else {
                w->SubtractAmount(rt->GetAmount());
            }

            // Mark as done for this period
            rt->SetLastGeneratedDate(today);
            generatedCount++;
            std::cout << "[Auto] Generated: " << rt->GetDescription() << " (" << rt->GetAmount() << ")\n";
        }
    }

    if (generatedCount == 0) {
        std::cout << "[System] No new recurring transactions due today.\n";
    }
}

ArrayList<Transaction*>* AppController::GetTransactionsByDateRange(Date start, Date end) {
    ArrayList<Transaction*>* result = new ArrayList<Transaction*>();

    for (size_t i = 0; i < transactions->Count(); ++i) {
        Transaction* t = transactions->Get(i);
        // Simple date range filter
        if (t->GetDate() >= start && t->GetDate() <= end) {
            result->Add(t);
        }
    }
    return result;
}
