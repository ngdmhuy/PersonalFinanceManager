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


void NavigationController::ShowIncomeFlow() {
    bool inIncomeMenu = true;

    while (inIncomeMenu) {
        char c = menus.DisplayIncomeMenu();

        if (c == 27) { // ESC - back to main menu
            break;
        }

        switch (c) {
            case '1':
                HandleAddIncome();
                break;
            case '2':
                HandleViewIncome();
                break;
            case '3':
                HandleEditIncome();
                break;
            case '4':
                HandleDeleteIncome();
                break;
            default:
                view.ShowError("Invalid selection. Try again.");
                PauseWithMessage("Press any key to continue...");
                break;
        }
    }
}

void NavigationController::HandleAddIncome() {
    if (!appController) {
        view.ShowError("Controller not available.");
        PauseWithMessage("Press any key to continue...");
        return;
    }

    view.ClearScreen();
    view.PrintHeader("ADD INCOME");

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
        std::string data[] = {std::to_string(i + 1), w->GetName(), view.FormatCurrency((long)(w->GetBalance()))};
        view.PrintTableRow(data, widths, 3);
    }
    view.PrintTableSeparator(widths, 3);

    view.MoveToXY(5, 9 + (int)wallets->Count());
    int walletIdx = InputValidator::GetValidIndex("Select index (1-" + std::to_string(static_cast<int>(wallets->Count())) + ") (0 to quit): ", 1, static_cast<int>(wallets->Count()), 5, 9 + static_cast<int>(wallets->Count()));
    if (walletIdx == 0) { view.ShowInfo("Selection cancelled."); PauseWithMessage("Press any key to continue..."); return; }
    Wallet* selectedWallet = wallets->Get(walletIdx - 1);
    std::string walletId = selectedWallet->GetId();

    // Step 2: Get Amount
    view.ClearScreen();
    view.PrintHeader("ADD INCOME - AMOUNT");
    double amount = InputValidator::GetValidMoney("Enter amount: ");

    // Step 3: Get Date
    view.ClearScreen();
    view.PrintHeader("ADD INCOME - DATE");
    Date date = InputValidator::GetValidDate("Enter date (YYYY-MM-DD): ");

    // Step 4: Get Description
    view.ClearScreen();
    view.PrintHeader("ADD INCOME - DESCRIPTION");
    std::string description = InputValidator::GetValidString("Enter description: ");

    // Step 5: Select Income Source
    ArrayList<IncomeSource*>* sources = appController->GetIncomeSourcesList();
    if (!sources || sources->Count() == 0) {
        view.ShowError("No income sources available. Please create an income source first.");
        PauseWithMessage("Press any key to continue...");
        return;
    }

    view.ClearScreen();
    view.PrintHeader("ADD INCOME - SOURCE");
    view.MoveToXY(5, 4);
    std::cout << "Available Income Sources:" << std::endl;

    std::string srcHeaders[] = {"Index", "Source Name"};
    int srcWidths[] = {10, 40};
    view.PrintTableHeader(srcHeaders, srcWidths, 2);

    for (size_t i = 0; i < sources->Count(); ++i) {
        IncomeSource* src = sources->Get(i);
        std::string srcData[] = {std::to_string(i + 1), src->GetName()};
        view.PrintTableRow(srcData, srcWidths, 2);
    }
    view.PrintTableSeparator(srcWidths, 2);

    view.MoveToXY(5, 9 + (int)sources->Count());
    int srcIdx = InputValidator::GetValidIndex("Select index (1-" + std::to_string(static_cast<int>(sources->Count())) + ") (0 to quit): ", 1, static_cast<int>(sources->Count()), 5, 9 + static_cast<int>(sources->Count()));
    if (srcIdx == 0) { view.ShowInfo("Selection cancelled."); PauseWithMessage("Press any key to continue..."); return; }
    IncomeSource* selectedSource = sources->Get(srcIdx - 1);
    std::string sourceId = selectedSource->GetId();

    // Step 6: Add transaction to AppController
    try {
        appController->AddTransaction(amount, walletId, sourceId, TransactionType::Income, date, description);
        
        view.ClearScreen();
        view.PrintHeader("INCOME ADDED SUCCESSFULLY");
        view.MoveToXY(5, 5);
        view.ShowSuccess("Income recorded!");
        
        view.MoveToXY(5, 7);
        std::cout << "Wallet: " << selectedWallet->GetName() << std::endl;
        view.MoveToXY(5, 8);
        std::cout << "Amount: " << view.FormatCurrency(static_cast<long>(amount)) << std::endl;
        view.MoveToXY(5, 9);
        std::cout << "Source: " << selectedSource->GetName() << std::endl;
        view.MoveToXY(5, 10);
        std::cout << "Date: " << date.ToString() << std::endl;
        view.MoveToXY(5, 11);
        std::cout << "Description: " << description << std::endl;
        
        PauseWithMessage("Press any key to return to menu...");
    } catch (const std::exception& e) {
        view.ShowError(std::string("Error adding income: ") + e.what());
        PauseWithMessage("Press any key to continue...");
    }
}

void NavigationController::HandleViewIncome() {
    view.ClearScreen();
    view.PrintHeader("VIEW INCOME");

    ArrayList<Transaction*>* txs = appController->GetTransactions();
    if (!txs || txs->Count() == 0) {
        view.ShowInfo("No transactions available.");
        PauseWithMessage("Press any key to return...");
        return;
    }

    // Print only Income transactions
    std::string headers[] = {"Index", "ID", "Wallet", "Amount", "Date", "Desc"};
    int widths[] = {6, 18, 20, 15, 12, 30};
    view.PrintTableHeader(headers, widths, 6);

    int count = 0;
    for (size_t i = 0; i < txs->Count(); ++i) {
        Transaction* t = txs->Get(i);
        if (t->GetType() != TransactionType::Income) continue;
        ++count;
        std::string walletName = "-";
        Wallet* w = appController->GetWalletById(t->GetWalletId());
        if (w) walletName = w->GetName();
        std::string dateStr = t->GetDate().ToString();
        std::string desc = t->GetDescription();
        if ((int)desc.length() > 28) desc = desc.substr(0, 27) + "~";

        std::string data[] = {std::to_string(count), t->GetId(), walletName, view.FormatCurrency(static_cast<long>(t->GetAmount())), dateStr, desc};
        view.PrintTableRow(data, widths, 6);
    }

    if (count == 0) {
        view.PrintTableSeparator(widths, 6);
        view.ShowInfo("No income records found.");
        PauseWithMessage("Press any key to return...");
        return;
    }

    view.PrintTableSeparator(widths, 6);
    PauseWithMessage("Press any key to return...");
}

void NavigationController::HandleEditIncome() {
    if (!appController) {
        view.ShowError("Controller not available.");
        PauseWithMessage("Press any key to continue...");
        return;
    }

    view.ClearScreen();
    view.PrintHeader("EDIT INCOME");

    ArrayList<Transaction*>* txs = appController->GetTransactions();
    if (!txs || txs->Count() == 0) {
        view.ShowInfo("No transactions available.");
        PauseWithMessage("Press any key to return...");
        return;
    }

    // Build list of income transactions with indices using ArrayList
    ArrayList<Transaction*>* incomes = new ArrayList<Transaction*>();
    for (size_t i = 0; i < txs->Count(); ++i) {
        Transaction* t = txs->Get(i);
        if (t->GetType() == TransactionType::Income) incomes->Add(t);
    }

    if (!incomes || incomes->Count() == 0) {
        view.ShowInfo("No income records found.");
        PauseWithMessage("Press any key to return...");
        if (incomes) delete incomes;
        return;
    }

    std::string headers[] = {"Index", "ID", "Wallet", "Amount", "Date", "Desc"};
    int widths[] = {6, 18, 20, 15, 12, 30};
    view.PrintTableHeader(headers, widths, 6);
    for (size_t i = 0; i < incomes->Count(); ++i) {
        Transaction* t = incomes->Get(i);
        Wallet* w = appController->GetWalletById(t->GetWalletId());
        std::string walletName = w ? w->GetName() : "-";
        std::string data[] = {std::to_string(i + 1), t->GetId(), walletName, view.FormatCurrency(static_cast<long>(t->GetAmount())), t->GetDate().ToString(), t->GetDescription()};
        view.PrintTableRow(data, widths, 6);
    }
    view.PrintTableSeparator(widths, 6);

    view.MoveToXY(5, 9 + (int)incomes->Count());
    int idx = InputValidator::GetValidIndex("Select index (1-" + std::to_string(static_cast<int>(incomes->Count())) + ") (0 to quit): ", 1, static_cast<int>(incomes->Count()), 5, 9 + static_cast<int>(incomes->Count()));
    if (idx == 0) { view.ShowInfo("Selection cancelled."); delete incomes; PauseWithMessage("Press any key to continue..."); return; }
    Transaction* target = incomes->Get(idx - 1);

    // Show current values and prompt new ones
    view.ClearScreen();
    view.PrintHeader("EDIT INCOME - DETAILS");
    Wallet* w = appController->GetWalletById(target->GetWalletId());
    std::string walletName = w ? w->GetName() : "-";
    view.MoveToXY(5,5);
    std::cout << "ID: " << target->GetId() << std::endl;
    std::cout << "Wallet: " << walletName << std::endl;
    std::cout << "Current Amount: " << view.FormatCurrency(static_cast<long>(target->GetAmount())) << std::endl;
    std::cout << "Current Date: " << target->GetDate().ToString() << std::endl;
    std::cout << "Current Description: " << target->GetDescription() << std::endl;

    double newAmount = InputValidator::GetValidMoney("Enter new amount: ");
    Date newDate = InputValidator::GetValidDate("Enter new date (YYYY-MM-DD): ");
    std::string newDesc = InputValidator::GetValidString("Enter new description: ");

    bool ok = appController->EditTransaction(target->GetId(), newAmount, newDate, newDesc);
    if (ok) view.ShowSuccess("Income updated successfully.");
    else view.ShowError("Failed to update income.");

    PauseWithMessage("Press any key to continue...");
}

void NavigationController::HandleDeleteIncome() {
    if (!appController) {
        view.ShowError("Controller not available.");
        PauseWithMessage("Press any key to continue...");
        return;
    }

    view.ClearScreen();
    view.PrintHeader("DELETE INCOME");

    ArrayList<Transaction*>* txs = appController->GetTransactions();
    if (!txs || txs->Count() == 0) {
        view.ShowInfo("No transactions available.");
        PauseWithMessage("Press any key to return...");
        return;
    }

    // Build list of income transactions using ArrayList
    ArrayList<Transaction*>* incomes = new ArrayList<Transaction*>();
    for (size_t i = 0; i < txs->Count(); ++i) {
        Transaction* t = txs->Get(i);
        if (t->GetType() == TransactionType::Income) incomes->Add(t);
    }

    if (!incomes || incomes->Count() == 0) {
        view.ShowInfo("No income records found.");
        PauseWithMessage("Press any key to return...");
        if (incomes) delete incomes;
        return;
    }

    std::string headers[] = {"Index", "ID", "Wallet", "Amount", "Date", "Desc"};
    int widths[] = {6, 18, 20, 15, 12, 30};
    view.PrintTableHeader(headers, widths, 6);
    for (size_t i = 0; i < incomes->Count(); ++i) {
        Transaction* t = incomes->Get(i);
        Wallet* w = appController->GetWalletById(t->GetWalletId());
        std::string walletName = w ? w->GetName() : "-";
        std::string data[] = {std::to_string(i + 1), t->GetId(), walletName, view.FormatCurrency(static_cast<long>(t->GetAmount())), t->GetDate().ToString(), t->GetDescription()};
        view.PrintTableRow(data, widths, 6);
    }
    view.PrintTableSeparator(widths, 6);

    view.MoveToXY(5, 9 + (int)incomes->Count());
    int idx = InputValidator::GetValidIndex("Select index (1-" + std::to_string(static_cast<int>(incomes->Count())) + ") (0 to quit): ", 1, static_cast<int>(incomes->Count()), 5, 9 + static_cast<int>(incomes->Count()));
    if (idx == 0) { view.ShowInfo("Selection cancelled."); delete incomes; PauseWithMessage("Press any key to continue..."); return; }
    Transaction* target = incomes->Get(idx - 1);
    view.MoveToXY(5, 11 + (int)incomes->Count());
    std::cout << "Are you sure you want to delete this income? (Y/N): ";
    int ch = GetKeyPress();
    std::cout << std::endl;

    if (ch != 'y' && ch != 'Y') {
        view.ShowInfo("Deletion cancelled.");
        PauseWithMessage("Press any key to continue...");
        return;
    }

    bool ok = appController->DeleteTransaction(target->GetId());
    if (ok) view.ShowSuccess("Income deleted successfully.");
    else view.ShowError("Failed to delete income.");

    PauseWithMessage("Press any key to continue...");
}
