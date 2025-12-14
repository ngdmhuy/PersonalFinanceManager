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

// --- FILE PATH CONSTANTS ---
const std::string FILE_WALLETS = "data/wallets.bin";
const std::string FILE_CATEGORIES = "data/categories.bin";
const std::string FILE_SOURCES = "data/sources.bin";
const std::string FILE_TRANSACTIONS = "data/transactions.bin";
const std::string FILE_RECURRING = "data/recurring.bin";

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
    this->transactionsMap = new HashMap<std::string, Transaction*>();
    this->recurringTransactionsMap = new HashMap<std::string, RecurringTransaction*>();

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
    // Only delete the Map container, not the contents (already deleted via list)
    if (transactionsMap) delete transactionsMap;

    // 3. Clean up Recurring Transactions
    if (recurringTransactions) {
        for (size_t i = 0; i < recurringTransactions->Count(); ++i) delete recurringTransactions->Get(i);
        delete recurringTransactions;
    }
    if (recurringTransactionsMap) delete recurringTransactionsMap;

    // 4. Clean up Wallets
    if (walletsList) {
        for (size_t i = 0; i < walletsList->Count(); ++i) delete walletsList->Get(i);
        delete walletsList;
    }
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
    // Order: Categories -> Wallets -> Transactions
    
    // 1. Load CATEGORIES
    std::ifstream fCat(FILE_CATEGORIES, std::ios::binary);
    if (fCat.is_open()) {
        size_t count = BinaryFileHelper::ReadSizeT(fCat);
        for (size_t i = 0; i < count; ++i) {
            Category* c = Category::FromBinary(fCat);
            categoriesList->Add(c);
            categoriesMap->Put(c->GetId(), c);
        }
        fCat.close();
    }
    
    // 2. Load INCOME SOURCES
    std::ifstream fSrc(FILE_SOURCES, std::ios::binary);
    if (fSrc.is_open()) {
        size_t count = BinaryFileHelper::ReadSizeT(fSrc);
        for (size_t i = 0; i < count; ++i) {
            IncomeSource* s = IncomeSource::FromBinary(fSrc);
            incomeSourcesList->Add(s);
            incomeSourcesMap->Put(s->GetId(), s);
        }
        fSrc.close();
    }
    
    // 3. Load WALLETS
    std::ifstream fWal(FILE_WALLETS, std::ios::binary);
    if (fWal.is_open()) {
        size_t count = BinaryFileHelper::ReadSizeT(fWal);
        for (size_t i = 0; i < count; ++i) {
            Wallet* w = Wallet::FromBinary(fWal);
            walletsList->Add(w);
            walletsMap->Put(w->GetId(), w);
        }
        fWal.close();
    }
    
    // 4. Load TRANSACTIONS
    std::ifstream fTrx(FILE_TRANSACTIONS, std::ios::binary);
    if (fTrx.is_open()) {
        size_t count = BinaryFileHelper::ReadSizeT(fTrx);
        for (size_t i = 0; i < count; ++i) {
            Transaction* t = Transaction::FromBinary(fTrx);
            transactions->Add(t);
            transactionsMap->Put(t->GetId(), t);
        }
        fTrx.close();
    }
    
    // 5. Load RECURRING TRANSACTIONS
    std::ifstream fRec(FILE_RECURRING, std::ios::binary);
    if (fRec.is_open()) {
        size_t count = BinaryFileHelper::ReadSizeT(fRec);
        for (size_t i = 0; i < count; ++i) {
            RecurringTransaction* rt = RecurringTransaction::FromBinary(fRec);
            recurringTransactions->Add(rt);
            recurringTransactionsMap->Put(rt->GetId(), rt);
        }
        fRec.close();
    }
    
    std::cout << "[System] Data loaded from disk.\n";
}

void AppController::SaveData() {
    // 1. Save CATEGORIES
    std::ofstream fCat(FILE_CATEGORIES, std::ios::binary);
    if (fCat.is_open()) {
        size_t count = categoriesList->Count();
        BinaryFileHelper::WriteSizeT(fCat, count);
        for (size_t i = 0; i < count; ++i)
            categoriesList->Get(i)->ToBinary(fCat);
        fCat.close();
    }
    
    // 2. Save INCOME SOURCES
    std::ofstream fSrc(FILE_SOURCES, std::ios::binary);
    if (fSrc.is_open()) {
        size_t count = incomeSourcesList->Count();
        BinaryFileHelper::WriteSizeT(fSrc, count);
        for (size_t i = 0; i < count; ++i)
            incomeSourcesList->Get(i)->ToBinary(fSrc);
        fSrc.close();
    }
    
    // 3. Save WALLETS
    std::ofstream fWal(FILE_WALLETS, std::ios::binary);
    if (fWal.is_open()) {
        size_t count = walletsList->Count();
        BinaryFileHelper::WriteSizeT(fWal, count);
        for (size_t i = 0; i < count; ++i)
            walletsList->Get(i)->ToBinary(fWal);
        fWal.close();
    }
    
    // 4. Save TRANSACTIONS
    std::ofstream fTrx(FILE_TRANSACTIONS, std::ios::binary);
    if (fTrx.is_open()) {
        size_t count = transactions->Count();
        BinaryFileHelper::WriteSizeT(fTrx, count);
        for (size_t i = 0; i < count; ++i)
            transactions->Get(i)->ToBinary(fTrx);
        fTrx.close();
    }
    
    // 5. Save RECURRING TRANSACTIONS
    std::ofstream fRec(FILE_RECURRING, std::ios::binary);
    if (fRec.is_open()) {
        size_t count = recurringTransactions->Count();
        BinaryFileHelper::WriteSizeT(fRec, count);
        for (size_t i = 0; i < count; ++i)
            recurringTransactions->Get(i)->ToBinary(fRec);
        fRec.close();
    }
    
    std::cout << "[System] Data saved to disk.\n";
}

// ==========================================
// 3. WALLET LOGIC (BASIC)
// ==========================================

void AppController::AddWallet(const std::string& name, double initialBalance) {
    if (IsStringEmptyOrWhitespace(name)) {
        std::cout << "[Error] Wallet creation failed: Name cannot be empty.\n";
        return;
    }
    
    for (size_t i = 0; i < walletsList->Count(); ++i) {
        if (walletsList->Get(i)->GetName() == name) {
            std::cout << "[Warning] Wallet '" << name << "' already exists.\n";
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
// 4. MASTER DATA LOGIC (BASIC)
// ==========================================

void AppController::AddCategory(const std::string& name) {
    if (IsStringEmptyOrWhitespace(name)) {
        std::cout << "[Error] Category name cannot be empty.\n";
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
    std::string id;
    do {
        id = IdGenerator::GenerateId(prefix);
    } while (incomeSourcesMap->ContainsKey(id));
    
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
    if (amount <= 0) {
        std::cout << "[Error] Transaction amount must be positive.\n";
        return;
    }

    Wallet* wallet = GetWalletById(walletId);
    if (wallet == nullptr) {
        std::cout << "[Error] Wallet ID not found: " << walletId << "\n";
        return;
    }

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

    transactions->Add(newTrans);
    transactionsMap->Put(transId, newTrans);
    
    std::cout << "[Success] Transaction added. New Wallet Balance: "
              << std::fixed << std::setprecision(0) << wallet->GetBalance() << "\n";
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
        std::cout << "[Error] Transaction ID not found: " << transactionId << "\n";
        return false;
    }

    Wallet* w = GetWalletById(target->GetWalletId());
    if (w != nullptr) {
        if (target->GetType() == TransactionType::Income) {
            w->SubtractAmount(target->GetAmount());
        } else {
            w->AddAmount(target->GetAmount());
        }
        std::cout << "[Info] Wallet balance restored. New Balance: "
                  << std::fixed << std::setprecision(0) << w->GetBalance() << "\n";
    } else {
        std::cout << "[Warning] Linked Wallet not found. Balance not restored.\n";
    }

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
    std::string id;
    do {
        id = IdGenerator::GenerateId(prefix);
    } while (recurringTransactionsMap->ContainsKey(id));
    
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

        if (rt->ShouldGenerate(today)) {
            Wallet* w = GetWalletById(rt->GetWalletId());
            if (w == nullptr) continue;

            std::string prefix = EnumHelper::IdPrefixToString(IdPrefix::Transaction);
            std::string newTransId;
            do {
                newTransId = IdGenerator::GenerateId(prefix);
            } while (transactionsMap->ContainsKey(newTransId));
            
            Transaction* autoTrans = rt->GenerateTransaction(newTransId, today);
            transactions->Add(autoTrans);
            
            if (rt->GetType() == TransactionType::Income) {
                w->AddAmount(rt->GetAmount());
            } else {
                w->SubtractAmount(rt->GetAmount());
            }

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
        if (t->GetDate() >= start && t->GetDate() <= end) {
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
        std::cout << "[Error] Wallet ID not found: " << id << "\n";
        return;
    }

    if (IsStringEmptyOrWhitespace(newName)) {
         std::cout << "[Error] Update failed: New name cannot be empty.\n";
         return;
    }

    w->SetName(newName); 
    std::cout << "[Success] Wallet updated to: " << newName << "\n";
}

bool AppController::DeleteWallet(const std::string& id) {
    Wallet* w = GetWalletById(id);
    if (w == nullptr) {
        std::cout << "[Error] Wallet ID not found.\n";
        return false;
    }

    // Dependency Check: Transactions
    for (size_t i = 0; i < transactions->Count(); ++i) {
        if (transactions->Get(i)->GetWalletId() == id) {
            std::cout << "[Error] Cannot delete wallet. It contains existing transactions.\n";
            return false;
        }
    }
    
    // Dependency Check: Recurring
    for (size_t i = 0; i < recurringTransactions->Count(); ++i) {
        if (recurringTransactions->Get(i)->GetWalletId() == id) {
             std::cout << "[Error] Cannot delete wallet. It is used in a Recurring Transaction setup.\n";
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
    std::cout << "[Success] Wallet deleted successfully.\n";
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
                std::cout << "[Error] Cannot delete Category. Used in Transaction ID: " << t->GetId() << "\n";
                return false;
            }
        }
    }
    
    for (size_t i = 0; i < recurringTransactions->Count(); ++i) {
        RecurringTransaction* rt = recurringTransactions->Get(i);
        if (rt->GetType() == TransactionType::Expense && rt->GetCategoryId() == id) {
             std::cout << "[Error] Cannot delete Category. Used in a Recurring Transaction.\n";
             return false;
        }
    }

    Category* c = GetCategoryById(id);
    if (!c) {
        std::cout << "[Error] Category ID not found.\n";
        return false;
    }

    categoriesMap->Remove(id);
    
    int idx = -1;
    for(size_t i=0; i<categoriesList->Count(); ++i) 
        if(categoriesList->Get(i)->GetId() == id) idx = (int)i;
    
    if(idx != -1) categoriesList->RemoveAt(idx);
    
    delete c;
    std::cout << "[Success] Category deleted successfully.\n";
    return true;
}

bool AppController::DeleteIncomeSource(const std::string& id) {
    for (size_t i = 0; i < transactions->Count(); ++i) {
        Transaction* t = transactions->Get(i);
        if (t->GetType() == TransactionType::Income) {
            if (t->GetCategoryId() == id) {
                std::cout << "[Error] Cannot delete Source. Used in Transaction ID: " << t->GetId() << "\n";
                return false;
            }
        }
    }
    
    for (size_t i = 0; i < recurringTransactions->Count(); ++i) {
        RecurringTransaction* rt = recurringTransactions->Get(i);
        if (rt->GetType() == TransactionType::Income && rt->GetCategoryId() == id) {
             std::cout << "[Error] Cannot delete Source. Used in a Recurring Transaction.\n";
             return false;
        }
    }

    IncomeSource* s = GetIncomeSourceById(id);
    if (!s) return false;

    incomeSourcesMap->Remove(id);
    
    int idx = -1;
    for(size_t i=0; i<incomeSourcesList->Count(); ++i) 
        if(incomeSourcesList->Get(i)->GetId() == id) idx = (int)i;
    
    if(idx != -1) incomeSourcesList->RemoveAt(idx);
    
    delete s;
    std::cout << "[Success] Income Source deleted.\n";
    return true;
}

// ---------------------------------------------------------
// 7.3. TRANSACTION EDIT (ADVANCED)
// ---------------------------------------------------------

bool AppController::EditTransaction(const std::string& id, double newAmount, Date newDate, std::string newDesc) {
    if (newAmount <= 0) {
        std::cout << "[Error] Amount must be positive.\n";
        return false;
    }

    Transaction** tPtr = transactionsMap->Get(id);
    if (tPtr == nullptr) {
        std::cout << "[Error] Transaction ID not found: " << id << "\n";
        return false;
    }
    Transaction* target = *tPtr;

    Wallet* w = GetWalletById(target->GetWalletId());
    if (w == nullptr) {
        std::cout << "[Critical Error] Wallet linked to this transaction not found!\n";
        return false;
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

    std::cout << "[Success] Transaction updated. Wallet balance adjusted.\n";
    return true;
}

// =================================================================================
// PART 8: STATISTICS & FILTERING ENGINE
// =================================================================================

ArrayList<Transaction*>* AppController::GetTransactionsByWallet(const std::string& walletId) {
    ArrayList<Transaction*>* result = new ArrayList<Transaction*>();
    
    if (GetWalletById(walletId) == nullptr) {
        std::cout << "[Warning] Filter failed: Wallet ID not found.\n";
        return result;
    }

    for (size_t i = 0; i < transactions->Count(); ++i) {
        Transaction* t = transactions->Get(i);
        if (t->GetWalletId() == walletId) {
            result->Add(t);
        }
    }
    return result;
}

ArrayList<Transaction*>* AppController::GetTransactionsByCategory(const std::string& categoryId) {
    ArrayList<Transaction*>* result = new ArrayList<Transaction*>();

    for (size_t i = 0; i < transactions->Count(); ++i) {
        Transaction* t = transactions->Get(i);
        // Only Expenses have Categories
        if (t->GetType() == TransactionType::Expense && t->GetCategoryId() == categoryId) {
            result->Add(t);
        }
    }
    return result;
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