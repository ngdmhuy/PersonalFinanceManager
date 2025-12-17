#include "Controllers/NavigationController.h"
#include "Controllers/AppController.h"

#include "Utils/PlatformUtils.h"

// Include Models
#include "Models/Transaction.h"
#include "Models/Income.h"
#include "Models/Expense.h"
#include "Models/Wallet.h"
#include "Models/Category.h"
#include "Models/IncomeSource.h"
#include "Models/RecurringTransaction.h"

#include <iostream>

using namespace std;

void NavigationController::ShowExpenseFlow() {
    bool inExpenseMenu = true;
    
    while (inExpenseMenu) {
        char c = menus.DisplayExpenseMenu();
        
        if (c == 27) { // ESC - back to main menu
            break;
        }
        
        switch (c) {
            case '1':
                HandleAddExpense();
                break;
            case '2':
                HandleViewExpenses();
                break;
            case '3':
                HandleEditExpense();
                break;
            case '4':
                HandleDeleteExpense();
                break;
            default:
                view.ShowError("Invalid selection. Try again.");
                PauseWithMessage("Press any key to continue...");
                break;
        }
    }
}

void NavigationController::HandleAddExpense() {
    if (!appController) {
        view.ShowError("Controller not available.");
        PauseWithMessage("Press any key to continue...");
        return;
    }

    view.ClearScreen();
    view.PrintHeader("ADD EXPENSE");

    // Step 1: Select Wallet
    ArrayList<Wallet*>* wallets = appController->GetWalletsList();
    if (!wallets || wallets->Count() == 0) {
        view.ShowError("No wallets available. Please create a wallet first.");
        PauseWithMessage("Press any key to continue...");
        return;
    }

    view.MoveToXY(5, 4);
    cout << "Available Wallets:" << endl;
    
    string headers[] = {"Index", "Wallet Name", "Balance"};
    int widths[] = {10, 30, 20};
    view.PrintTableHeader(headers, widths, 3);

    for (size_t i = 0; i < wallets->Count(); ++i) {
        Wallet* w = wallets->Get(i);
        string data[] = {to_string(i + 1), w->GetName(), view.FormatCurrency(static_cast<long>(w->GetBalance()))};
        view.PrintTableRow(data, widths, 3);
    }
    view.PrintTableSeparator(widths, 3);

    view.MoveToXY(5, 5 + wallets->Count() + 3);
    cout << "Enter wallet index (1-" << wallets->Count() << "): ";
    int walletIdx;
    cin >> walletIdx;
    cin.ignore();

    if (walletIdx < 1 || walletIdx > static_cast<int>(wallets->Count())) {
        view.ShowError("Invalid wallet selection.");
        PauseWithMessage("Press any key to continue...");
        return;
    }

    Wallet* selectedWallet = wallets->Get(walletIdx - 1);
    string walletId = selectedWallet->GetId();

    // Step 2: Get Amount
    view.ClearScreen();
    view.PrintHeader("ADD EXPENSE - AMOUNT");
    double amount = InputValidator::GetValidMoney("Enter amount: ");

    // Step 3: Get Date
    view.ClearScreen();
    view.PrintHeader("ADD EXPENSE - DATE");
    Date date = InputValidator::GetValidDate("Enter date (DD/MM/YYYY): ");

    // Step 4: Get Description
    view.ClearScreen();
    view.PrintHeader("ADD EXPENSE - DESCRIPTION");
    string description = InputValidator::GetValidString("Enter description: ");

    // Step 5: Select Category
    ArrayList<Category*>* categories = appController->GetCategoriesList();
    if (!categories || categories->Count() == 0) {
        view.ShowError("No categories available. Please create a category first.");
        PauseWithMessage("Press any key to continue...");
        return;
    }

    view.ClearScreen();
    view.PrintHeader("ADD EXPENSE - CATEGORY");
    view.MoveToXY(5, 4);
    cout << "Available Categories:" << endl;

    string catHeaders[] = {"Index", "Category Name", ""};
    int catWidths[] = {10, 40, 20};
    view.PrintTableHeader(catHeaders, catWidths, 3);

    for (size_t i = 0; i < categories->Count(); ++i) {
        Category* cat = categories->Get(i);
        string catData[] = {to_string(i + 1), cat->GetName(), ""};
        view.PrintTableRow(catData, catWidths, 3);
    }
    view.PrintTableSeparator(catWidths, 3);

    view.MoveToXY(5, 5 + categories->Count() + 3);
    cout << "Enter category index (1-" << categories->Count() << "): ";
    int catIdx;
    cin >> catIdx;
    cin.ignore();

    if (catIdx < 1 || catIdx > static_cast<int>(categories->Count())) {
        view.ShowError("Invalid category selection.");
        PauseWithMessage("Press any key to continue...");
        return;
    }

    Category* selectedCategory = categories->Get(catIdx - 1);
    string categoryId = selectedCategory->GetId();

    // Step 6: Add transaction to AppController
    try {
        appController->AddTransaction(amount, walletId, categoryId, TransactionType::Expense, date, description);
        
        view.ClearScreen();
        view.PrintHeader("EXPENSE ADDED SUCCESSFULLY");
        view.MoveToXY(5, 5);
        view.ShowSuccess("Expense recorded!!");
        
        view.MoveToXY(5, 7);
        cout << "Wallet: " << selectedWallet->GetName() << endl;
        view.MoveToXY(5, 8);
        cout << "Amount: " << view.FormatCurrency(static_cast<long>(amount)) << endl;
        view.MoveToXY(5, 9);
        cout << "Category: " << selectedCategory->GetName() << endl;
        view.MoveToXY(5, 10);
        cout << "Date: " << date.ToString() << endl;
        view.MoveToXY(5, 11);
        cout << "Description: " << description << endl;
        
        PauseWithMessage("Press any key to return to menu...");
    } catch (const exception& e) {
        view.ShowError(string("Error adding expense: ") + e.what());
        PauseWithMessage("Press any key to continue...");
    }
}

void NavigationController::HandleViewExpenses() {
    view.ClearScreen();
    view.PrintHeader("VIEW EXPENSES");
    view.ShowInfo("Feature not yet implemented (placeholder)");
    PauseWithMessage("Press any key to return...");
}

void NavigationController::HandleEditExpense() {
    view.ClearScreen();
    view.PrintHeader("EDIT EXPENSE");
    view.ShowInfo("Feature not yet implemented (placeholder)");
    PauseWithMessage("Press any key to return...");
}

void NavigationController::HandleDeleteExpense() {
    view.ClearScreen();
    view.PrintHeader("DELETE EXPENSE");
    view.ShowInfo("Feature not yet implemented (placeholder)");
    PauseWithMessage("Press any key to return...");
}