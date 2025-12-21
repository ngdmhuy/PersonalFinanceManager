//
//  AppController.cpp
//  PersonalFinanceManager
//
//  Created by Le Tran Hoang Long (M2) on 6/12/25.
//  Implementation of Business Logic with strict Validation and Error Handling.
//

#include "Controllers/AppController.h"

// Include Utils
#include "Utils/IdGenerator.h"
#include "Utils/AppHelpers.h"

// Include Models
#include "Models/Transaction.h"
#include "Models/Income.h"
#include "Models/Expense.h"
#include "Models/Wallet.h"
#include "Models/Category.h"
#include "Models/IncomeSource.h"
#include "Models/RecurringTransaction.h"

#include <iostream>
#include <fstream>
#include <iomanip>
#include <algorithm>

// --- FILE PATH CONSTANTS ---
const std::string FILE_WALLETS = "data/wallets.bin";
const std::string FILE_CATEGORIES = "data/categories.bin";
const std::string FILE_SOURCES = "data/sources.bin";
const std::string FILE_TRANSACTIONS = "data/transactions.bin";
const std::string FILE_RECURRING = "data/recurring.bin";

using namespace AppHelpers;

// --- [HELPER] Indexing Logic ---

void AppController::AddTransactionToIndex(Transaction* t) {
    // 1. Index by Wallet
    AddToIndexMap(walletIndex, t->GetWalletId(), t);

    // 2. Index by Category (Only for Expense)
    if (t->GetType() == TransactionType::Expense)
        AddToIndexMap(categoryIndex, t->GetCategoryId(), t);
    
    // 3. Index by Income Sources
    if (t->GetType() == TransactionType::Income)
        AddToIndexMap(incomeSourceIndex, t->GetCategoryId(), t);
}

void AppController::RemoveTransactionFromIndex(Transaction* t) {
    // 1. Remove from Wallet Index
    RemoveFromIndexMap(walletIndex, t->GetWalletId(), t);

    // 2. Remove from Category Index
    if (t->GetType() == TransactionType::Expense)
        RemoveFromIndexMap(categoryIndex, t->GetCategoryId(), t);
    
    // 3. Remove from Income Source Index
    if (t->GetType() == TransactionType::Income)
        RemoveFromIndexMap(incomeSourceIndex, t->GetCategoryId(), t);
}

// ==========================================
// 1. CONSTRUCTOR & DESTRUCTOR
// ==========================================

AppController::AppController(ConsoleView* v) : view(v) {
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
    this->transactionsMap = new HashMap<std::string, Transaction*>();
    this->recurringTransactionsMap = new HashMap<std::string, RecurringTransaction*>();

    // 3. Load existing data
    LoadData();

    // --- [NEW] Initialize Indices ---
    this->walletIndex = new HashMap<std::string, ArrayList<Transaction*>*>();
    this->categoryIndex = new HashMap<std::string, ArrayList<Transaction*>*>();
    this->incomeSourceIndex = new HashMap<std::string, ArrayList<Transaction*>*>();

    // Rebuild indices from loaded data
    for (size_t i = 0; i < transactions->Count(); ++i) {
        AddTransactionToIndex(transactions->Get(i));
    }
    
    ProcessRecurringTransactions();
}

AppController::~AppController() {
    // Save state before closing
    SaveData();
    
    ClearIndexMap(walletIndex);
    ClearIndexMap(categoryIndex);
    ClearIndexMap(incomeSourceIndex);
    
    FreeList(transactions);
    FreeList(recurringTransactions);
    FreeList(walletsList);
    FreeList(categoriesList);
    FreeList(incomeSourcesList);
    
    if (transactionsMap) delete transactionsMap;
    if (recurringTransactionsMap) delete recurringTransactionsMap;
    if (walletsMap) delete walletsMap;
    if (categoriesMap) delete categoriesMap;
    if (incomeSourcesMap) delete incomeSourcesMap;
    
    if (view) view->ShowInfo("Memory cleaned up successfully.");
}

// ==========================================
// 2. DATA PERSISTENCE
// ==========================================

void AppController::SaveData() {
    // --- [UPDATE] Sort before saving ---
    // Sort transactions by date so next load is already sorted
    transactions->Sort(CompareTransactionsByDate);

    SaveTable(FILE_CATEGORIES, categoriesList);
    SaveTable(FILE_SOURCES, incomeSourcesList);
    SaveTable(FILE_WALLETS, walletsList);
    SaveTable(FILE_TRANSACTIONS, transactions);
    SaveTable(FILE_RECURRING, recurringTransactions);
    
    if (view) view->ShowSuccess("Data saved to disk.");
}

void AppController::LoadData() {
    // Order: Categories -> Wallets -> Transactions
    LoadTable(FILE_CATEGORIES, categoriesList, categoriesMap);
    LoadTable(FILE_SOURCES, incomeSourcesList, incomeSourcesMap);
    LoadTable(FILE_WALLETS, walletsList, walletsMap);
    LoadTable(FILE_TRANSACTIONS, transactions, transactionsMap);
    LoadTable(FILE_RECURRING, recurringTransactions, recurringTransactionsMap);
    
    if (view) view->ShowSuccess("Data loaded from disk.");
}

// ==========================================
// 3. WALLET LOGIC (BASIC)
// ==========================================

void AppController::AddWallet(const std::string& name, double initialBalance) {
    if (IsStringEmptyOrWhitespace(name)) {
        if (view) view->ShowError("Wallet creation failed: Name cannot be empty.");
        return;
    }
    
    for (size_t i = 0; i < walletsList->Count(); ++i) {
        if (walletsList->Get(i)->GetName() == name) {
            if (view) view->ShowWarning("Wallet '" + name + "' already exists.");
            break;
        }
    }

    std::string prefix = EnumHelper::IdPrefixToString(IdPrefix::Wallet);
    std::string newId;
    do {
        newId = IdGenerator::GenerateId(prefix);
    } while (walletsMap->ContainsKey(newId));

    Wallet* newWallet = new Wallet(newId, name, initialBalance);

    walletsMap->Put(newId, newWallet);
    walletsList->Add(newWallet);
    
    if (view) view->ShowSuccess("Wallet created: " + name);
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
// 4. MASTER DATA LOGIC (BASIC)
// ==========================================

void AppController::AddCategory(const std::string& name) {
    if (IsStringEmptyOrWhitespace(name)) {
        if (view) view->ShowError("Category name cannot be empty.");
        return;
    }

    std::string prefix = EnumHelper::IdPrefixToString(IdPrefix::Category);
    std::string id;
    do {
        id = IdGenerator::GenerateId(prefix);
    } while (categoriesMap->ContainsKey(id));
    
    Category* obj = new Category(id, name);
    categoriesMap->Put(id, obj);
    categoriesList->Add(obj);

    if (view) view->ShowSuccess("Category created: " + name);
}

Category* AppController::GetCategoryById(const std::string& id) {
    Category** c = categoriesMap->Get(id);
    return (c != nullptr) ? *c : nullptr;
}

void AppController::AddIncomeSource(const std::string& name) {
    if (IsStringEmptyOrWhitespace(name)) {
        if (view) view->ShowError("Source name cannot be empty.");
        return;
    }
    
    std::string prefix = EnumHelper::IdPrefixToString(IdPrefix::IncomeSource);
    std::string id;
    do {
        id = IdGenerator::GenerateId(prefix);
    } while (incomeSourcesMap->ContainsKey(id));
    
    IncomeSource* obj = new IncomeSource(id, name);
    incomeSourcesMap->Put(id, obj);
    incomeSourcesList->Add(obj);

    if (view) view->ShowSuccess("Income Source created: " + name);
}

IncomeSource* AppController::GetIncomeSourceById(const std::string& id) {
    IncomeSource** s = incomeSourcesMap->Get(id);
    return (s != nullptr) ? *s : nullptr;
}

// ==========================================
// 5. TRANSACTION CORE LOGIC
// ==========================================

void AppController::AddTransaction(double amount, std::string walletId, std::string categoryOrSourceId, TransactionType type, Date date, std::string description) {
    if (amount <= 0) {
        if (view) view->ShowError("Transaction amount must be positive.");
        return;
    }

    Wallet* wallet = GetWalletById(walletId);
    if (wallet == nullptr) {
        if (view) view->ShowError("Wallet ID not found: " + walletId);
        return;
    }
    
    if (type == TransactionType::Expense) {
        if (GetCategoryById(categoryOrSourceId) == nullptr) {
             if (view) view->ShowError("Expense Category not found.");
             return;
        }
    } else {
        if (GetIncomeSourceById(categoryOrSourceId) == nullptr) {
            if (view) view->ShowError("Income Source not found.");
            return;
        }
    }

    std::string prefix = EnumHelper::IdPrefixToString(IdPrefix::Transaction);
    std::string transId;
    do {
        transId = IdGenerator::GenerateId(prefix);
    } while (transactionsMap->ContainsKey(transId));

    Transaction* newTrans = nullptr;

    if (type == TransactionType::Income) {
        newTrans = new Income(transId, walletId, categoryOrSourceId, amount, date, description);
        wallet->AddAmount(amount);
    } else {
        newTrans = new Expense(transId, walletId, categoryOrSourceId, amount, date, description);
        wallet->SubtractAmount(amount);
    }
    
    size_t insertPos = GetSortedInsertIndex(transactions, newTrans->GetDate());
    transactions->Insert(insertPos, newTrans);
    

    AddTransactionToIndex(newTrans); // <--- Update Index
    transactionsMap->Put(transId, newTrans);
    
    if (view) view->ShowSuccess("Transaction added. New Wallet Balance: " + std::to_string(static_cast<long long>(wallet->GetBalance())));
}

bool AppController::DeleteTransaction(const std::string& transactionId) {
    int foundIndex = -1;
    Transaction* target = nullptr;

    for (size_t i = 0; i < transactions->Count(); ++i) {
        if (transactions->Get(i)->GetId() == transactionId) {
            foundIndex = (int)i;
            target = transactions->Get(i);
            break;
        }
    }

    if (foundIndex == -1 || target == nullptr) {
        if (view) view->ShowError("Transaction ID not found: " + transactionId);
        return false;
    }

    Wallet* w = GetWalletById(target->GetWalletId());
    if (w != nullptr) {
        if (target->GetType() == TransactionType::Income) {
            w->SubtractAmount(target->GetAmount());
        } else {
            w->AddAmount(target->GetAmount());
        }
        if (view) view->ShowInfo("Wallet balance restored. New Balance: " + std::to_string(static_cast<long long>(w->GetBalance())));
    } else {
        if (view) view->ShowWarning("Linked Wallet not found. Balance not restored.");
    }

    RemoveTransactionFromIndex(target); // <--- Update Index
    transactions->RemoveAt(foundIndex);
    transactionsMap->Remove(transactionId);
    delete target;
    return true;
}

// ==========================================
// 6. AUTOMATION & REPORTING
// ==========================================

void AppController::AddRecurringTransaction(Frequency freq, Date startDate, Date endDate, std::string walletId, std::string categoryId, double amount, TransactionType type, std::string desc) {
    if (endDate.IsValid() && startDate > endDate) {
        if (view) view->ShowError("Invalid Date Range: Start > End.");
        return;
    }
    
    if (GetWalletById(walletId) == nullptr) {
        if (view) view->ShowError("Wallet ID not found.");
        return;
    }

    if (amount <= 0) {
        if (view) view->ShowError("Amount must be positive.");
        return;
    }

    std::string prefix = EnumHelper::IdPrefixToString(IdPrefix::Recurring);
    std::string id;
    do {
        id = IdGenerator::GenerateId(prefix);
    } while (recurringTransactionsMap->ContainsKey(id));
    
    RecurringTransaction* rt = new RecurringTransaction(id, freq, startDate, endDate, walletId, categoryId, amount, type, desc);
    recurringTransactions->Add(rt);
    recurringTransactionsMap->Put(id, rt);
    
    ProcessRecurringTransactions();
    if (view) view->ShowSuccess("Recurring transaction scheduled.");
}

RecurringTransaction* AppController::GetRecurringById(const std::string& id) {
    RecurringTransaction** r = recurringTransactionsMap->Get(id);
    return (r != nullptr) ? *r : nullptr;
}

bool AppController::DeleteRecurringTransaction(const std::string& id) {
    RecurringTransaction* r = GetRecurringById(id);
    if (r == nullptr) {
        if (view) view->ShowError("Recurring transaction ID not found: " + id);
        return false;
    }

    // Remove from list
    int foundIndex = -1;
    for (size_t i = 0; i < recurringTransactions->Count(); ++i) {
        if (recurringTransactions->Get(i)->GetId() == id) {
            foundIndex = static_cast<int>(i);
            break;
        }
    }
    if (foundIndex >= 0) {
        recurringTransactions->RemoveAt(foundIndex);
    }

    // Remove from map and delete object
    recurringTransactionsMap->Remove(id);
    delete r;

    if (view) view->ShowSuccess("Recurring transaction deleted: " + id);
    return true;
}

void AppController::EditRecurringTransaction(const std::string& id, Frequency freq, Date startDate, Date endDate, std::string walletId, std::string categoryId, double amount, TransactionType type, std::string desc) {
    RecurringTransaction* r = GetRecurringById(id);
    if (r == nullptr) {
        if (view) view->ShowError("Recurring transaction ID not found: " + id);
        return;
    }

    if (endDate.IsValid() && startDate > endDate) {
        if (view) view->ShowError("Invalid Date Range: Start > End.");
        return;
    }

    if (GetWalletById(walletId) == nullptr) {
        if (view) view->ShowError("Wallet ID not found.");
        return;
    }

    if (amount <= 0) {
        if (view) view->ShowError("Amount must be positive.");
        return;
    }

    r->SetFrequency(freq);
    r->SetStartDate(startDate);
    r->SetEndDate(endDate);
    r->SetWalletId(walletId);
    r->SetCategoryId(categoryId);
    r->SetAmount(amount);
    r->SetDescription(desc);
    
    ProcessRecurringTransactions();

    if (view) view->ShowSuccess("Recurring transaction updated: " + id);
}

void AppController::ProcessRecurringTransactions() {
    Date today = Date::GetTodayDate();
    int generatedCount = 0;

    if (view) view->ShowInfo("Checking recurring transactions...");

    for (size_t i = 0; i < recurringTransactions->Count(); ++i) {
        RecurringTransaction* rt = recurringTransactions->Get(i);

        while (rt->ShouldGenerate(today)) {
            Wallet* w = GetWalletById(rt->GetWalletId());
            if (w == nullptr) break;
            
            std::string prefix = EnumHelper::IdPrefixToString(IdPrefix::Transaction);
            std::string newTransId;
            do {
                newTransId = IdGenerator::GenerateId(prefix);
            } while (transactionsMap->ContainsKey(newTransId));
            
            Transaction* autoTrans = rt->GenerateTransaction(newTransId, rt->GetNextDueDate());
            
            size_t pos = GetSortedInsertIndex(transactions, autoTrans->GetDate());
            transactions->Insert(pos, autoTrans);
            AddTransactionToIndex(autoTrans);
            
            if (rt->GetType() == TransactionType::Income) {
                w->AddAmount(rt->GetAmount());
            } else {
                w->SubtractAmount(rt->GetAmount());
            }
            
            rt->SetLastGeneratedDate(rt->GetNextDueDate());
            ++generatedCount;
            if (view) view->ShowSuccess("Generated: " + rt->GetDescription() + " (" + std::to_string(static_cast<long long>(rt->GetAmount())) + ")");
        }
    }

    if (generatedCount == 0) {
        if (view) view->ShowInfo("No new recurring transactions due today.");
    }
}

// --- [OPTIMIZATION] Binary Search for Date Range ---
ArrayList<Transaction*>* AppController::GetTransactionsByDateRange(Date start, Date end) {
    ArrayList<Transaction*>* result = new ArrayList<Transaction*>();

    if (transactions->Count() == 0) return result;

    // 2. BINARY SEARCH (Tìm vị trí phần tử đầu tiên có Date >= start)
    // Thuật toán: Lower Bound
    int low = 0;
    int high = static_cast<int>(transactions->Count()) - 1;
    int startIndex = -1;

    while (low <= high) {
        int mid = low + (high - low) / 2;
        Transaction* midItem = transactions->Get(mid);

        if (midItem->GetDate() >= start) {
            startIndex = mid;   // Có thể đây là đáp án, nhưng thử tìm bên trái xem còn không
            high = mid - 1;
        } else {
            low = mid + 1;      // Ngày bé hơn start -> Tìm bên phải
        }
    }

    // 3. Duyệt từ startIndex và dừng ngay khi vượt quá endDate (Early Exit)
    if (startIndex != -1) {
        for (size_t i = static_cast<size_t>(startIndex); i < transactions->Count(); ++i) {
            Transaction* t = transactions->Get(i);
            
            // Nếu ngày hiện tại đã lớn hơn ngày kết thúc -> Dừng ngay lập tức
            if (t->GetDate() > end) {
                break; 
            }
            
            // Chắc chắn t nằm trong khoảng [start, end] vì logic trên
            result->Add(t);
        }
    }

    return result;
}

// =================================================================================
// PART 7: ADVANCED CRUD (EDIT & SECURE DELETE)
// =================================================================================

// ---------------------------------------------------------
// 7.1. WALLET MANAGEMENT (ADVANCED)
// ---------------------------------------------------------

void AppController::EditWallet(const std::string& id, const std::string& newName) {
    Wallet* w = GetWalletById(id);
    if (w == nullptr) {
        if (view) view->ShowError("Wallet ID not found: " + id);
        return;
    }

    if (IsStringEmptyOrWhitespace(newName)) {
         if (view) view->ShowError("Update failed: New name cannot be empty.");
         return;
    }

    w->SetName(newName); 
    if (view) view->ShowSuccess("Wallet updated to: " + newName);
}

bool AppController::DeleteWallet(const std::string& id) {
    Wallet* w = GetWalletById(id);
    if (w == nullptr) {
        if (view) view->ShowError("Wallet ID not found.");
        return false;
    }

    // Dependency Check: Transactions
    for (size_t i = 0; i < transactions->Count(); ++i) {
        if (transactions->Get(i)->GetWalletId() == id) {
            if (view) view->ShowError("Cannot delete wallet. It contains existing transactions.");
            return false;
        }
    }
    
    // Dependency Check: Recurring
    for (size_t i = 0; i < recurringTransactions->Count(); ++i) {
        if (recurringTransactions->Get(i)->GetWalletId() == id) {
             if (view) view->ShowError("Cannot delete wallet. It is used in a Recurring Transaction setup.");
             return false;
        }
    }

    walletsMap->Remove(id);
    
    int indexToRemove = -1;
    for (size_t i = 0; i < walletsList->Count(); ++i) {
        if (walletsList->Get(i)->GetId() == id) {
            indexToRemove = (int)i;
            break;
        }
    }
    if (indexToRemove != -1) walletsList->RemoveAt(indexToRemove);

    delete w; 
    if (view) view->ShowSuccess("Wallet deleted successfully.");
    return true;
}

// ---------------------------------------------------------
// 7.2. MASTER DATA (ADVANCED)
// ---------------------------------------------------------

bool AppController::DeleteCategory(const std::string& id) {
    for (size_t i = 0; i < transactions->Count(); ++i) {
        Transaction* t = transactions->Get(i);
        if (t->GetType() == TransactionType::Expense) {
            if (t->GetCategoryId() == id) {
                if (view) view->ShowError("Cannot delete Category. Used in Transaction ID: " + t->GetId());
                return false;
            }
        }
    }
    
    for (size_t i = 0; i < recurringTransactions->Count(); ++i) {
        RecurringTransaction* rt = recurringTransactions->Get(i);
        if (rt->GetType() == TransactionType::Expense && rt->GetCategoryId() == id) {
             if (view) view->ShowError("Cannot delete Category. Used in a Recurring Transaction.");
             return false;
        }
    }

    Category* c = GetCategoryById(id);
    if (!c) {
        if (view) view->ShowError("Category ID not found.");
        return false;
    }

    categoriesMap->Remove(id);
    
    int idx = -1;
    for(size_t i=0; i<categoriesList->Count(); ++i) 
        if(categoriesList->Get(i)->GetId() == id) idx = (int)i;
    
    if(idx != -1) categoriesList->RemoveAt(idx);
    
    delete c;
    if (view) view->ShowSuccess("Category deleted successfully.");
    return true;
}

void AppController::EditCategory(const std::string& id, const std::string& newName) {
    Category* c = GetCategoryById(id);
    if (!c) { if (view) view->ShowError("Category ID not found."); return; }
    if (IsStringEmptyOrWhitespace(newName)) { if (view) view->ShowError("Category name cannot be empty."); return; }
    c->SetName(newName);
    if (view) view->ShowSuccess("Category updated.");
}

void AppController::EditIncomeSource(const std::string& id, const std::string& newName) {
    IncomeSource* s = GetIncomeSourceById(id);
    if (!s) { if (view) view->ShowError("Source ID not found."); return; }
    if (IsStringEmptyOrWhitespace(newName)) { if (view) view->ShowError("Source name cannot be empty."); return; }
    s->SetName(newName);
    if (view) view->ShowSuccess("Income Source updated.");

}

bool AppController::DeleteIncomeSource(const std::string& id) {
    for (size_t i = 0; i < transactions->Count(); ++i) {
        Transaction* t = transactions->Get(i);
        if (t->GetType() == TransactionType::Income) {
            if (t->GetCategoryId() == id) {
                if (view) view->ShowError("Cannot delete Source. Used in Transaction ID: " + t->GetId());
                return false;
            }
        }
    }
    
    for (size_t i = 0; i < recurringTransactions->Count(); ++i) {
        RecurringTransaction* rt = recurringTransactions->Get(i);
        if (rt->GetType() == TransactionType::Income && rt->GetCategoryId() == id) {
             if (view) view->ShowError("Cannot delete Source. Used in a Recurring Transaction.");
             return false;
        }
    }

    IncomeSource* s = GetIncomeSourceById(id);
    if (!s) {
        if (view) view->ShowError("Source ID not found.");
        return false;
    }

    incomeSourcesMap->Remove(id);
    
    int idx = -1;
    for(size_t i=0; i<incomeSourcesList->Count(); ++i) 
        if(incomeSourcesList->Get(i)->GetId() == id) idx = (int)i;
    
    if(idx != -1) incomeSourcesList->RemoveAt(idx);
    
    delete s;
    if (view) view->ShowSuccess("Income Source deleted.");
    return true;
}

// ---------------------------------------------------------
// 7.3. TRANSACTION EDIT (ADVANCED)
// ---------------------------------------------------------

bool AppController::EditTransaction(const std::string& id, double newAmount, Date newDate, std::string newDesc) {
    if (newAmount <= 0) {
        if (view) view->ShowError("Amount must be positive.");
        return false;
    }

    Transaction** tPtr = transactionsMap->Get(id);
    if (tPtr == nullptr) {
        if (view) view->ShowError("Transaction ID not found: " + id);
        return false;
    }
    Transaction* target = *tPtr;

    Wallet* w = GetWalletById(target->GetWalletId());
    if (w == nullptr) {
        if (view) view->ShowError("Wallet linked to this transaction not found!");
        return false;
    }
    
    bool dateChanged = (target->GetDate()) != newDate;
    
    if (dateChanged) {
        RemoveTransactionFromIndex(target);
        transactions->Remove(target);
    }

    // RE-CALCULATE BALANCE
    // 1. Undo Old
    if (target->GetType() == TransactionType::Income) {
        w->SubtractAmount(target->GetAmount()); 
    } else {
        w->AddAmount(target->GetAmount());      
    }

    // 2. Apply New
    if (target->GetType() == TransactionType::Income) {
        w->AddAmount(newAmount); 
    } else {
        w->SubtractAmount(newAmount); 
    }

    target->SetAmount(newAmount);
    target->SetDate(newDate);
    target->SetDescription(newDesc);
    
    if (dateChanged) {
        size_t newPos = GetSortedInsertIndex(transactions, newDate);
        transactions->Insert(newPos, target);
        AddTransactionToIndex(target);
    }

    if (view) view->ShowSuccess("Transaction updated. Wallet balance adjusted.");
    return true;
}

// =================================================================================
// PART 8: STATISTICS & FILTERING ENGINE
// =================================================================================

ArrayList<Transaction*>* AppController::GetTransactionsByWallet(const std::string& walletId) {
    // --- [OPTIMIZED] Use HashMap Index ---
    if (walletIndex->ContainsKey(walletId)) {
        // Return the cached list directly!
        return *walletIndex->Get(walletId);
    }
    // Return empty list if not found
    return new ArrayList<Transaction*>(); 
}

ArrayList<Transaction*>* AppController::GetTransactionsByCategory(const std::string& categoryId) {
    // --- [OPTIMIZED] Use HashMap Index ---
    if (categoryIndex->ContainsKey(categoryId)) {
        return *categoryIndex->Get(categoryId);
    }
    return new ArrayList<Transaction*>();
}

ArrayList<Transaction*>* AppController::SearchTransactions(const std::string& keyword) {
    ArrayList<Transaction*>* result = new ArrayList<Transaction*>();
    
    for (size_t i = 0; i < transactions->Count(); ++i) {
        Transaction* t = transactions->Get(i);
        if (t->GetDescription().find(keyword) != std::string::npos) {
            result->Add(t);
        }
    }
    return result;
}
