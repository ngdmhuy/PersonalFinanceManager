//
//  Enums.h
//  PersonalFinanceManager
//
//  Created by Nguyen Dinh Minh Huy on 27/12/25.
//

#include "Controllers/NavigationController.h"
#include "Controllers/AppController.h"
#include "Views/InputValidator.h"
#include "Utils/PlatformUtils.h"

// Include Models
#include "Models/Transaction.h"
#include "Models/Wallet.h"
#include "Models/Category.h"
#include "Models/IncomeSource.h"

#include <iostream>
#include <iomanip>

void NavigationController::PrintTransactionList(ArrayList<Transaction*>* list) {
    if (!list || list->Count() == 0) {
        view.ShowInfo("No transactions found matching your criteria.");
        return;
    }

    // 1. Cập nhật Header để thêm cột Wallet và Category/Source
    // Bỏ cột ID đi vì người dùng thường không cần xem ID, họ cần xem tên
    std::string headers[] = {"Date", "Wallet", "Category/Source", "Amount", "Type", "Description"};
    
    // Tăng số lượng cột lên 6 và canh chỉnh độ rộng
    int numCols = 6;
    int widths[] = {12, 20, 20, 15, 8, 25}; 

    view.PrintTableHeader(headers, widths, numCols);

    for (size_t i = 0; i < list->Count(); ++i) {
        Transaction* t = list->Get(i);

        // 2. Lấy tên Wallet từ ID
        Wallet* w = appController->GetWalletById(t->GetWalletId());
        std::string wName = (w != nullptr) ? w->GetName() : "Unknown";

        // 3. Lấy tên Category (nếu là Expense) hoặc Source (nếu là Income)
        std::string catName = "Unknown";
        std::string typeStr = "";
        
        if (t->GetType() == TransactionType::Income) {
            typeStr = "Income";
            IncomeSource* s = appController->GetIncomeSourceById(t->GetCategoryId());
            catName = (s != nullptr) ? s->GetName() : "Unknown";
        } else {
            typeStr = "Expense";
            Category* c = appController->GetCategoryById(t->GetCategoryId());
            catName = (c != nullptr) ? c->GetName() : "Unknown";
        }

        // 4. Định dạng dữ liệu để in ra bảng
        std::string data[] = {
            t->GetDate().ToString(),                        // Date
            wName,                                          // Wallet Name (Thay vì ID)
            catName,                                        // Category/Source Name
            view.FormatCurrency((long long)t->GetAmount()), // Amount có dấu phẩy
            typeStr,                                        // Type
            t->GetDescription()                             // Description
        };

        view.PrintTableRow(data, widths, numCols);
    }
    
    view.PrintTableSeparator(widths, numCols);
}

// =============================================================
// MAIN SEARCH FLOW
// =============================================================
void NavigationController::ShowSearchFlow() {
    while (true) {
        char c = menus.DisplaySearchMenu();
        if (c == 27) break;

        switch (c) {
            case '1':
                HandleSearchByKeyword();
                break;
            case '2':
                HandleFilterByWallet();
                break;
            case '3':
                HandleFilterByCategory();
                break;
            case '4':
                HandleFilterBySource();
                break;
            case '5':
                HandleFilterByAmount();
                break;
            case '6':
                HandleFilterByDate();
                break;
            default:
                view.ShowError("Invalid selection.");
                PauseWithMessage("Press any key to continue...");
                break;
        }
    }
}

// KEYWORD SEARCH
void NavigationController::HandleSearchByKeyword() {
    view.ClearScreen();
    view.PrintHeader("SEARCH BY KEYWORD");
    view.PrintText("Search for Transaction ID or words in Description.");
    
    std::string keyword = InputValidator::GetValidString("Enter keyword: ");
    
    ArrayList<Transaction*>* results = appController->SearchTransactions(keyword);
    
    view.PrintText("");
    PrintTransactionList(results);
    
    delete results;
    PauseWithMessage("Press any key to continue...");
}

// FILTER BY WALLET
void NavigationController::HandleFilterByWallet() {
    view.ClearScreen();
    view.PrintHeader("FILTER BY WALLET");
    
    ArrayList<Wallet*>* wallets = appController->GetWalletsList();
    if (!wallets || wallets->Count() == 0) {
        view.ShowError("No wallets available.");
        PauseWithMessage("Press any key to continue...");
        return;
    }

    // Show List
    view.PrintText("Select a Wallet to filter by:");
    for (size_t i = 0; i < wallets->Count(); ++i) {
        std::cout << "  " << (i + 1) << ". " << wallets->Get(i)->GetName() << " (" << view.FormatCurrency((long long)wallets->Get(i)->GetBalance()) << ")" << std::endl;
    }
    std::cout << std::endl;

    // Get Selection
    int idx = InputValidator::GetValidIndex("Select Wallet (1-" + std::to_string(wallets->Count()) + "): ", 1, (int)wallets->Count());
    std::string walletId = wallets->Get(idx - 1)->GetId();
    std::string walletName = wallets->Get(idx - 1)->GetName();

    // Fetch Results
    view.ClearScreen();
    view.PrintHeader("TRANSACTIONS IN: " + walletName);
    
    ArrayList<Transaction*>* results = appController->GetTransactionsByWallet(walletId);
    PrintTransactionList(results);
    
    delete results;
    PauseWithMessage("Press any key to continue...");
}

// FILTER BY CATEGORY
void NavigationController::HandleFilterByCategory() {
    view.ClearScreen();
    view.PrintHeader("FILTER BY CATEGORY");
    
    ArrayList<Category*>* cats = appController->GetCategoriesList();
    if (!cats || cats->Count() == 0) {
        view.ShowError("No categories available.");
        PauseWithMessage("Press any key to continue...");
        return;
    }

    view.PrintText("Select a Category (Expenses only):");
    for (size_t i = 0; i < cats->Count(); ++i) {
        std::cout << "  " << (i + 1) << ". " << cats->Get(i)->GetName() << std::endl;
    }
    std::cout << std::endl;

    int idx = InputValidator::GetValidIndex("Select Category (1-" + std::to_string(cats->Count()) + "): ", 1, (int)cats->Count());
    std::string catId = cats->Get(idx - 1)->GetId();
    std::string catName = cats->Get(idx - 1)->GetName();

    view.ClearScreen();
    view.PrintHeader("EXPENSES IN: " + catName);
    
    ArrayList<Transaction*>* results = appController->GetTransactionsByCategory(catId);
    PrintTransactionList(results);
    
    delete results;
    PauseWithMessage("Press any key to continue...");
}

// FILTER BY INCOME SOURCE
void NavigationController::HandleFilterBySource() {
    view.ClearScreen();
    view.PrintHeader("FILTER BY INCOME SOURCE");
    
    ArrayList<IncomeSource*>* sources = appController->GetIncomeSourcesList();
    if (!sources || sources->Count() == 0) {
        view.ShowError("No income sources available.");
        PauseWithMessage("Press any key to continue...");
        return;
    }

    view.PrintText("Select a Source (Incomes only):");
    for (size_t i = 0; i < sources->Count(); ++i) {
        std::cout << "  " << (i + 1) << ". " << sources->Get(i)->GetName() << std::endl;
    }
    std::cout << std::endl;

    int idx = InputValidator::GetValidIndex("Select Source (1-" + std::to_string(sources->Count()) + "): ", 1, (int)sources->Count());
    std::string sourceId = sources->Get(idx - 1)->GetId();
    std::string sourceName = sources->Get(idx - 1)->GetName();

    view.ClearScreen();
    view.PrintHeader("INCOMES FROM: " + sourceName);
    
    ArrayList<Transaction*>* results = appController->GetTransactionsByIncomeSource(sourceId);
    PrintTransactionList(results);
    
    delete results;
    PauseWithMessage("Press any key to continue...");
}

// FILTER BY AMOUNT RANGE
void NavigationController::HandleFilterByAmount() {
    view.ClearScreen();
    view.PrintHeader("FILTER BY AMOUNT RANGE");
    
    view.PrintText("Enter the minimum and maximum transaction amount.");
    
    double min = InputValidator::GetValidMoney("Enter Min Amount: ");
    double max = InputValidator::GetValidMoney("Enter Max Amount: ");

    if (min > max) {
        view.ShowError("Error: Minimum amount cannot be greater than Maximum amount.");
        PauseWithMessage("Press any key to try again...");
        return;
    }

    view.ClearScreen();
    view.PrintHeader("TRANSACTIONS: " + view.FormatCurrency((long long)min) + " - " + view.FormatCurrency((long long)max));
    
    ArrayList<Transaction*>* results = appController->GetTransactionsByAmountRange(min, max);
    PrintTransactionList(results);
    
    delete results;
    PauseWithMessage("Press any key to continue...");
}

// FILTER BY DATE RANGE
void NavigationController::HandleFilterByDate() {
    view.ClearScreen();
    view.PrintHeader("FILTER BY DATE RANGE");

    Date start = InputValidator::GetValidDate("Enter Start Date (YYYY-MM-DD) or 'T' for today: ");
    Date end   = InputValidator::GetValidDate("Enter End Date (YYYY-MM-DD) or 'T' for today: ");

    if (start > end) {
        view.ShowError("Start Date cannot be after End Date.");
        PauseWithMessage("Press any key to retry...");
        return;
    }

    view.ClearScreen();
    view.PrintHeader("TRANSACTIONS: " + start.ToString() + " - " + end.ToString());
    
    ArrayList<Transaction*>* results = appController->GetTransactionsByDateRange(start, end);
    PrintTransactionList(results);
    
    delete results;
    PauseWithMessage("Press any key to continue...");
}
