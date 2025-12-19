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
            case '5':
                ShowCategoryFlow();
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
    view.PrintText("Available Wallets:");
    
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
    int walletIdx = InputValidator::GetValidIndex("Select index (1-" + std::to_string(static_cast<int>(wallets->Count())) + ") (0 to cancel): ", 1, static_cast<int>(wallets->Count()), 5, 9 + static_cast<int>(wallets->Count()));
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
    Date date = InputValidator::GetValidDate("Enter date (YYYY-MM-DD) or 'T' for today: ");

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
    view.PrintText("Available Categories:");

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
    int catIdx = InputValidator::GetValidIndex("Select index (1-" + std::to_string(static_cast<int>(categories->Count())) + ") (0 to cancel): ", 1, static_cast<int>(categories->Count()), 5, 9 + static_cast<int>(categories->Count()));
    if (catIdx == 0) { view.ShowInfo("Selection cancelled."); PauseWithMessage("Press any key to continue..."); return; }
    Category* selectedCategory = categories->Get(catIdx - 1);
    std::string categoryId = selectedCategory->GetId();

    // Step 6: Add transaction to AppController
    try {
        appController->AddTransaction(amount, walletId, categoryId, TransactionType::Expense, date, description);
        
        view.ClearScreen();
        view.PrintHeader("EXPENSE ADDED SUCCESSFULLY");
        view.MoveToXY(5, 5);
        
        view.MoveToXY(5, 7);
        view.PrintText("Wallet: " + selectedWallet->GetName());
        view.MoveToXY(5, 8);
        view.PrintText("Amount: " + view.FormatCurrency(static_cast<long>(amount)));
        view.MoveToXY(5, 9);
        view.PrintText("Category: " + selectedCategory->GetName());
        view.MoveToXY(5, 10);
        view.PrintText("Date: " + date.ToString());
        view.MoveToXY(5, 11);
        view.PrintText("Description: " + description);
        
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
    view.PrintText("Current Amount: " + view.FormatCurrency(static_cast<long>(target->GetAmount())));
    view.PrintText("Current Date   : " + target->GetDate().ToString());
    view.PrintText("Current Desc   : " + target->GetDescription());

    // Get new values
    double newAmount = InputValidator::GetValidMoney("Enter new amount: ");
    Date newDate = InputValidator::GetValidDate("Enter new date (YYYY-MM-DD) or 'T' for today: ");
    std::string newDesc = InputValidator::GetValidString("Enter new description: ");

    bool ok = appController->EditTransaction(target->GetId(), newAmount, newDate, newDesc);
    (void)ok; // AppController emits feedback; avoid duplicate messages.

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
    int idx = InputValidator::GetValidIndex("Select index (1-" + std::to_string(static_cast<int>(expenses->Count())) + ") (0 to cancel): ", 1, static_cast<int>(expenses->Count()), 5, 9 + static_cast<int>(expenses->Count()));
    if (idx == 0) { view.ShowInfo("Selection cancelled."); delete expenses; PauseWithMessage("Press any key to return..."); return; }
    Transaction* target = expenses->Get(idx - 1);

    view.MoveToXY(5, 11 + (int)expenses->Count());
    std::cout << "Are you sure you want to delete this expense? (Y/N): ";
    int ch = GetKeyPress();
    view.PrintText("");

    if (ch != 'y' && ch != 'Y') {
        view.ShowInfo("Deletion cancelled.");
        delete expenses;
        PauseWithMessage("Press any key to return...");
        return;
    }

    appController->DeleteTransaction(target->GetId());

    delete expenses;
    PauseWithMessage("Press any key to return...");
}

// ===== CATEGORY MANAGEMENT =====
void NavigationController::ShowCategoryFlow() {
    bool inCatMenu = true;
    while (inCatMenu) {
        char c = menus.DisplayCategoryMenu();
        if (c == 27) break;
        switch (c) {
            case '1': HandleCreateCategory(); break;
            case '2': HandleViewCategories(); break;
            case '3': HandleEditCategory(); break;
            case '4': HandleDeleteCategory(); break;
            default: view.ShowError("Invalid selection. Try again."); PauseWithMessage("Press any key to continue..."); break;
        }
    }
}

void NavigationController::HandleCreateCategory() {
    view.ClearScreen();
    view.PrintHeader("CREATE CATEGORY");
    std::string name = InputValidator::GetValidString("Enter category name (0 to cancel): ");
    if (name == "0") { view.ShowInfo("Category creation cancelled."); PauseWithMessage("Press any key to continue..."); return; }
    appController->AddCategory(name);
    PauseWithMessage("Press any key to continue...");
}

void NavigationController::HandleViewCategories() {
    view.ClearScreen();
    view.PrintHeader("CATEGORIES");
    ArrayList<Category*>* cats = appController->GetCategoriesList();
    if (!cats || cats->Count() == 0) { view.ShowInfo("No categories available."); PauseWithMessage("Press any key to continue..."); return; }

    std::string headers[] = {"Index", "ID", "Name"};
    int widths[] = {8, 20, 40};
    view.PrintTableHeader(headers, widths, 3);
    for (size_t i=0;i<cats->Count();++i) {
        Category* c = cats->Get(i);
        std::string data[] = {std::to_string((int)i+1), c->GetId(), c->GetName()};
        view.PrintTableRow(data, widths, 3);
    }
    view.PrintTableSeparator(widths, 3);
    PauseWithMessage("Press any key to continue...");
}

void NavigationController::HandleEditCategory() {
    view.ClearScreen();
    view.PrintHeader("EDIT CATEGORY");
    ArrayList<Category*>* cats = appController->GetCategoriesList();
    if (!cats || cats->Count() == 0) { view.ShowInfo("No categories to edit."); PauseWithMessage("Press any key to continue..."); return; }

    std::string headers[] = {"Index", "ID", "Name"};
    int widths[] = {8, 20, 40};
    view.PrintTableHeader(headers, widths, 3);
    for (size_t i=0;i<cats->Count();++i) {
        Category* c = cats->Get(i);
        std::string data[] = {std::to_string((int)i+1), c->GetId(), c->GetName()};
        view.PrintTableRow(data, widths, 3);
    }
    view.PrintTableSeparator(widths, 3);

    view.MoveToXY(5, 9 + (int)cats->Count());
    int idx = InputValidator::GetValidIndex("Select index (1-" + std::to_string((int)cats->Count()) + ") (0 to cancel): ", 1, (int)cats->Count(), 5, 9 + (int)cats->Count());
    if (idx == 0) { view.ShowInfo("Selection cancelled."); PauseWithMessage("Press any key to continue..."); return; }

    Category* target = cats->Get(idx-1);
    std::string newName = InputValidator::GetValidString("Enter new name: ");
    appController->EditCategory(target->GetId(), newName);
    PauseWithMessage("Press any key to continue...");
}

void NavigationController::HandleDeleteCategory() {
    view.ClearScreen();
    view.PrintHeader("DELETE CATEGORY");
    ArrayList<Category*>* cats = appController->GetCategoriesList();
    if (!cats || cats->Count() == 0) { view.ShowInfo("No categories to delete."); PauseWithMessage("Press any key to continue..."); return; }

    std::string headers[] = {"Index", "ID", "Name"};
    int widths[] = {8, 20, 40};
    view.PrintTableHeader(headers, widths, 3);
    for (size_t i=0;i<cats->Count();++i) {
        Category* c = cats->Get(i);
        std::string data[] = {std::to_string((int)i+1), c->GetId(), c->GetName()};
        view.PrintTableRow(data, widths, 3);
    }
    view.PrintTableSeparator(widths, 3);

    view.MoveToXY(5, 9 + (int)cats->Count());
    int idx = InputValidator::GetValidIndex("Select index to delete (1-" + std::to_string((int)cats->Count()) + ") (0 to cancel): ", 1, (int)cats->Count(), 5, 9 + (int)cats->Count());
    if (idx == 0) { view.ShowInfo("Deletion cancelled."); PauseWithMessage("Press any key to continue..."); return; }

    Category* target = cats->Get(idx-1);
    view.MoveToXY(5, 11 + (int)cats->Count());
    std::cout << "Are you sure you want to delete category '" << target->GetName() << "'? (Y/N): ";
    int ch = GetKeyPress();
    view.PrintText("");
    if (ch != 'y' && ch != 'Y') { view.ShowInfo("Deletion cancelled."); PauseWithMessage("Press any key to continue..."); return; }

    appController->DeleteCategory(target->GetId());
    PauseWithMessage("Press any key to continue...");
}