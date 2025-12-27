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

    std::string headers[] = {"ID", "Date", "Amount", "Type", "Description"};
    int widths[] = {18, 12, 15, 8, 30};
    view.PrintTableHeader(headers, widths, 5);

    for (size_t i = 0; i < list->Count(); ++i) {
        Transaction* t = list->Get(i);
        std::string typeStr = (t->GetType() == TransactionType::Income) ? "Income" : "Expense";
        
        std::string dateStr = t->GetDate().ToString();
        std::string amtStr = view.FormatCurrency(static_cast<long long>(t->GetAmount()));
        std::string desc = t->GetDescription();
        
        if (desc.length() > 28) desc = desc.substr(0, 25) + "...";

        std::string data[] = { t->GetId(), dateStr, amtStr, typeStr, desc };
        view.PrintTableRow(data, widths, 5);
    }
    view.PrintTableSeparator(widths, 5);
    
    view.ShowSuccess("Found " + std::to_string(list->Count()) + " transaction(s).");
}

// =============================================================
// MAIN SEARCH FLOW
// =============================================================
void NavigationController::ShowSearchFlow() {
    while (true) {
        char c = menus.DisplaySearchMenu();
        if (c == 27) break;

        switch (c) {
            case '1': HandleSearchByKeyword(); break;
            case '2': HandleFilterByWallet(); break;
            case '3': HandleFilterByCategory(); break;
            case '4': HandleFilterBySource(); break;
            case '5': HandleFilterByAmount(); break;
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
    
    delete results; // Clean up
    PauseWithMessage("Press any key to continue...");
}
