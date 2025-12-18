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
    std::cout << "Available Wallets:" << std::endl;
    
    std::string headers[] = {"Index", "Wallet Name", "Balance"};
    int widths[] = {10, 30, 20};
    view.PrintTableHeader(headers, widths, 3);

    for (size_t i = 0; i < wallets->Count(); ++i) {
        Wallet* w = wallets->Get(i);
        std::string data[] = {std::to_string(i + 1), w->GetName(), view.FormatCurrency(static_cast<long>(w->GetBalance()))};
        view.PrintTableRow(data, widths, 3);
    }
    view.PrintTableSeparator(widths, 3);

    // Select wallet using robust integer input
    view.MoveToXY(5, 9 + (int)wallets->Count());
    int walletIdx = InputValidator::GetValidIndex("Select index (1-" + std::to_string(static_cast<int>(wallets->Count())) + ") (0 to quit): ", 1, static_cast<int>(wallets->Count()), 5, 9 + static_cast<int>(wallets->Count()));
    if (walletIdx == 0) { view.ShowInfo("Selection cancelled."); PauseWithMessage("Press any key to continue..."); return; }
    Wallet* selectedWallet = wallets->Get(walletIdx - 1);
    std::string walletId = selectedWallet->GetId();

    // Step 2: Get Amount
    view.ClearScreen();
    view.PrintHeader("ADD EXPENSE - AMOUNT");
    double amount = InputValidator::GetValidMoney("Enter amount: ");

    // Step 3: Get Date
    view.ClearScreen();
    view.PrintHeader("ADD EXPENSE - DATE");
    Date date = InputValidator::GetValidDate("Enter date (YYYY-MM-DD): ");

    // Step 4: Get Description
    view.ClearScreen();
    view.PrintHeader("ADD EXPENSE - DESCRIPTION");
    std::string description = InputValidator::GetValidString("Enter description: ");

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
    std::cout << "Available Categories:" << std::endl;

    std::string catHeaders[] = {"Index", "Category Name", ""};
    int catWidths[] = {10, 40, 20};
    view.PrintTableHeader(catHeaders, catWidths, 3);

    for (size_t i = 0; i < categories->Count(); ++i) {
        Category* cat = categories->Get(i);
        std::string catData[] = {std::to_string(i + 1), cat->GetName(), ""};
        view.PrintTableRow(catData, catWidths, 3);
    }
    view.PrintTableSeparator(catWidths, 3);

    // Select category using robust integer input
    view.MoveToXY(5, 9 + (int)categories->Count());
    int catIdx = InputValidator::GetValidIndex("Select index (1-" + std::to_string(static_cast<int>(categories->Count())) + ") (0 to quit): ", 1, static_cast<int>(categories->Count()), 5, 9 + static_cast<int>(categories->Count()));
    if (catIdx == 0) { view.ShowInfo("Selection cancelled."); PauseWithMessage("Press any key to continue..."); return; }
    Category* selectedCategory = categories->Get(catIdx - 1);
    std::string categoryId = selectedCategory->GetId();

    // Step 6: Add transaction to AppController
    try {
        appController->AddTransaction(amount, walletId, categoryId, TransactionType::Expense, date, description);
        
        view.ClearScreen();
        view.PrintHeader("EXPENSE ADDED SUCCESSFULLY");
        view.MoveToXY(5, 5);
        view.ShowSuccess("Expense recorded!!");
        
        view.MoveToXY(5, 7);
        std::cout << "Wallet: " << selectedWallet->GetName() << std::endl;
        view.MoveToXY(5, 8);
        std::cout << "Amount: " << view.FormatCurrency(static_cast<long>(amount)) << std::endl;
        view.MoveToXY(5, 9);
        std::cout << "Category: " << selectedCategory->GetName() << std::endl;
        view.MoveToXY(5, 10);
        std::cout << "Date: " << date.ToString() << std::endl;
        view.MoveToXY(5, 11);
        std::cout << "Description: " << description << std::endl;
        
        PauseWithMessage("Press any key to return to menu...");
    } catch (const std::exception& e) {
        view.ShowError(std::string("Error adding expense: ") + e.what());
        PauseWithMessage("Press any key to continue...");
    }
}

void NavigationController::HandleViewExpenses() {
    if (!appController) {
        view.ShowError("Controller not available.");
        PauseWithMessage("Press any key to continue...");
        return;
    }

    view.ClearScreen();
    view.PrintHeader("VIEW EXPENSES");

    ArrayList<Transaction*>* all = appController->GetTransactions();
    ArrayList<Transaction*>* expenses = new ArrayList<Transaction*>();

    for (size_t i = 0; i < all->Count(); ++i) {
        Transaction* t = all->Get(i);
        if (t->GetType() == TransactionType::Expense) expenses->Add(t);
    }

    if (expenses->Count() == 0) {
        view.ShowInfo("No expenses recorded.");
        delete expenses;
        PauseWithMessage("Press any key to return...");
        return;
    }

    std::string headers[] = {"Index", "Summary", "Amount"};
    int widths[] = {6, 70, 20};
    view.PrintTableHeader(headers, widths, 3);

    for (size_t i = 0; i < expenses->Count(); ++i) {
        Transaction* t = expenses->Get(i);
        Wallet* w = appController->GetWalletById(t->GetWalletId());
        Category* c = appController->GetCategoryById(t->GetCategoryId());
        std::string walletName = (w != nullptr) ? w->GetName() : "Unknown Wallet";
        std::string catName = (c != nullptr) ? c->GetName() : "Unknown Category";

        std::string summary = walletName + " | " + catName + " | " + t->GetDate().ToString() + " | " + t->GetDescription();
        std::string data[] = {std::to_string(i + 1), summary, view.FormatCurrency(static_cast<long>(t->GetAmount()))};
        view.PrintTableRow(data, widths, 3);
    }

    view.PrintTableSeparator(widths, 3);
    delete expenses;
    PauseWithMessage("Press any key to return...");
}

void NavigationController::HandleEditExpense() {
    if (!appController) {
        view.ShowError("Controller not available.");
        PauseWithMessage("Press any key to continue...");
        return;
    }

    view.ClearScreen();
    view.PrintHeader("EDIT EXPENSE");

    ArrayList<Transaction*>* all = appController->GetTransactions();
    ArrayList<Transaction*>* expenses = new ArrayList<Transaction*>();

    for (size_t i = 0; i < all->Count(); ++i) {
        Transaction* t = all->Get(i);
        if (t->GetType() == TransactionType::Expense) expenses->Add(t);
    }

    if (expenses->Count() == 0) {
        view.ShowInfo("No expenses to edit.");
        delete expenses;
        PauseWithMessage("Press any key to return...");
        return;
    }

    std::string headers[] = {"Index", "Summary", "Amount"};
    int widths[] = {6, 70, 20};
    view.PrintTableHeader(headers, widths, 3);

    for (size_t i = 0; i < expenses->Count(); ++i) {
        Transaction* t = expenses->Get(i);
        Wallet* w = appController->GetWalletById(t->GetWalletId());
        Category* c = appController->GetCategoryById(t->GetCategoryId());
        std::string walletName = (w != nullptr) ? w->GetName() : "Unknown Wallet";
        std::string catName = (c != nullptr) ? c->GetName() : "Unknown Category";

        std::string summary = walletName + " | " + catName + " | " + t->GetDate().ToString() + " | " + t->GetDescription();
        std::string data[] = {std::to_string(i + 1), summary, view.FormatCurrency(static_cast<long>(t->GetAmount()))};
        view.PrintTableRow(data, widths, 3);
    }

    view.PrintTableSeparator(widths, 3);

    view.MoveToXY(5, 9 + (int)expenses->Count());
    int idx = InputValidator::GetValidIndex("Select index (1-" + std::to_string(static_cast<int>(expenses->Count())) + "): ", 1, static_cast<int>(expenses->Count()), 5, 9 + static_cast<int>(expenses->Count()));
    if (idx == 0) { view.ShowInfo("Selection cancelled."); delete expenses; PauseWithMessage("Press any key to return..."); return; }
    Transaction* target = expenses->Get(idx - 1);

    // Show current values
    view.ClearScreen();
    view.PrintHeader("EDIT EXPENSE - CURRENT");
    std::cout << "Current Amount: " << view.FormatCurrency(static_cast<long>(target->GetAmount())) << std::endl;
    std::cout << "Current Date   : " << target->GetDate().ToString() << std::endl;
    std::cout << "Current Desc   : " << target->GetDescription() << std::endl;

    // Get new values
    double newAmount = InputValidator::GetValidMoney("Enter new amount: ");
    Date newDate = InputValidator::GetValidDate("Enter new date (YYYY-MM-DD): ");
    std::string newDesc = InputValidator::GetValidString("Enter new description: ");

    bool ok = appController->EditTransaction(target->GetId(), newAmount, newDate, newDesc);
    if (ok) view.ShowSuccess("Expense updated successfully.");
    else view.ShowError("Failed to update expense. Check logs.");

    delete expenses;
    PauseWithMessage("Press any key to return...");
}

void NavigationController::HandleDeleteExpense() {
    if (!appController) {
        view.ShowError("Controller not available.");
        PauseWithMessage("Press any key to continue...");
        return;
    }

    view.ClearScreen();
    view.PrintHeader("DELETE EXPENSE");

    ArrayList<Transaction*>* all = appController->GetTransactions();
    ArrayList<Transaction*>* expenses = new ArrayList<Transaction*>();

    for (size_t i = 0; i < all->Count(); ++i) {
        Transaction* t = all->Get(i);
        if (t->GetType() == TransactionType::Expense) expenses->Add(t);
    }

    if (expenses->Count() == 0) {
        view.ShowInfo("No expenses to delete.");
        delete expenses;
        PauseWithMessage("Press any key to return...");
        return;
    }

    std::string headers[] = {"Index", "Summary", "Amount"};
    int widths[] = {6, 70, 20};
    view.PrintTableHeader(headers, widths, 3);

    for (size_t i = 0; i < expenses->Count(); ++i) {
        Transaction* t = expenses->Get(i);
        Wallet* w = appController->GetWalletById(t->GetWalletId());
        Category* c = appController->GetCategoryById(t->GetCategoryId());
        std::string walletName = (w != nullptr) ? w->GetName() : "Unknown Wallet";
        std::string catName = (c != nullptr) ? c->GetName() : "Unknown Category";

        std::string summary = walletName + " | " + catName + " | " + t->GetDate().ToString() + " | " + t->GetDescription();
        std::string data[] = {std::to_string(i + 1), summary, view.FormatCurrency(static_cast<long>(t->GetAmount()))};
        view.PrintTableRow(data, widths, 3);
    }

    view.PrintTableSeparator(widths, 3);

    view.MoveToXY(5, 9 + (int)expenses->Count());
    int idx = InputValidator::GetValidIndex("Select index (1-" + std::to_string(static_cast<int>(expenses->Count())) + ") (0 to quit): ", 1, static_cast<int>(expenses->Count()), 5, 9 + static_cast<int>(expenses->Count()));
    if (idx == 0) { view.ShowInfo("Selection cancelled."); delete expenses; PauseWithMessage("Press any key to return..."); return; }
    Transaction* target = expenses->Get(idx - 1);

    view.MoveToXY(5, 11 + (int)expenses->Count());
    std::cout << "Are you sure you want to delete this expense? (Y/N): ";
    int ch = GetKeyPress();
    std::cout << std::endl;

    if (ch != 'y' && ch != 'Y') {
        view.ShowInfo("Deletion cancelled.");
        delete expenses;
        PauseWithMessage("Press any key to return...");
        return;
    }

    bool ok = appController->DeleteTransaction(target->GetId());
    if (ok) view.ShowSuccess("Expense deleted successfully.");
    else view.ShowError("Failed to delete expense. Check for issues.");

    delete expenses;
    PauseWithMessage("Press any key to return...");
}