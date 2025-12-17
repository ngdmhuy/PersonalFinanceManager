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
    cout << "Available Wallets:" << endl;
    
    string headers[] = {"Index", "Wallet Name", "Balance"};
    int widths[] = {10, 30, 20};
    view.PrintTableHeader(headers, widths, 3);

    for (size_t i = 0; i < wallets->Count(); ++i) {
        Wallet* w = wallets->Get(i);
        string data[] = {to_string(i + 1), w->GetName(), view.FormatCurrency((long)(w->GetBalance()))};
        view.PrintTableRow(data, widths, 3);
    }
    view.PrintTableSeparator(widths, 3);

    view.MoveToXY(5, 5 + wallets->Count() + 3);
    cout << "Enter wallet index (1-" << wallets->Count() << "): ";
    int walletIdx;
    cin >> walletIdx;
    cin.ignore();

    if (walletIdx < 1 || walletIdx > (int)(wallets->Count())) {
        view.ShowError("Invalid wallet selection.");
        PauseWithMessage("Press any key to continue...");
        return;
    }

    Wallet* selectedWallet = wallets->Get(walletIdx - 1);
    string walletId = selectedWallet->GetId();

    // Step 2: Get Amount
    view.ClearScreen();
    view.PrintHeader("ADD INCOME - AMOUNT");
    double amount = InputValidator::GetValidMoney("Enter amount: ");

    // Step 3: Get Date
    view.ClearScreen();
    view.PrintHeader("ADD INCOME - DATE");
    Date date = InputValidator::GetValidDate("Enter date (DD/MM/YYYY): ");

    // Step 4: Get Description
    view.ClearScreen();
    view.PrintHeader("ADD INCOME - DESCRIPTION");
    string description = InputValidator::GetValidString("Enter description: ");

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
    cout << "Available Income Sources:" << endl;

    string srcHeaders[] = {"Index", "Source Name", ""};
    int srcWidths[] = {10, 40, 20};
    view.PrintTableHeader(srcHeaders, srcWidths, 3);

    for (size_t i = 0; i < sources->Count(); ++i) {
        IncomeSource* src = sources->Get(i);
        string srcData[] = {to_string(i + 1), src->GetName(), ""};
        view.PrintTableRow(srcData, srcWidths, 3);
    }
    view.PrintTableSeparator(srcWidths, 3);

    view.MoveToXY(5, 5 + sources->Count() + 3);
    cout << "Enter source index (1-" << sources->Count() << "): ";
    int srcIdx;
    cin >> srcIdx;
    cin.ignore();

    if (srcIdx < 1 || srcIdx > static_cast<int>(sources->Count())) {
        view.ShowError("Invalid source selection.");
        PauseWithMessage("Press any key to continue...");
        return;
    }

    IncomeSource* selectedSource = sources->Get(srcIdx - 1);
    string sourceId = selectedSource->GetId();

    // Step 6: Add transaction to AppController
    try {
        appController->AddTransaction(amount, walletId, sourceId, TransactionType::Income, date, description);
        
        view.ClearScreen();
        view.PrintHeader("INCOME ADDED SUCCESSFULLY");
        view.MoveToXY(5, 5);
        view.ShowSuccess("Income recorded!");
        
        view.MoveToXY(5, 7);
        cout << "Wallet: " << selectedWallet->GetName() << endl;
        view.MoveToXY(5, 8);
        cout << "Amount: " << view.FormatCurrency(static_cast<long>(amount)) << endl;
        view.MoveToXY(5, 9);
        cout << "Source: " << selectedSource->GetName() << endl;
        view.MoveToXY(5, 10);
        cout << "Date: " << date.ToString() << endl;
        view.MoveToXY(5, 11);
        cout << "Description: " << description << endl;
        
        PauseWithMessage("Press any key to return to menu...");
    } catch (const exception& e) {
        view.ShowError(string("Error adding income: ") + e.what());
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
    string headers[] = {"Index", "ID", "Wallet", "Amount", "Date", "Desc"};
    int widths[] = {6, 18, 20, 15, 12, 30};
    view.PrintTableHeader(headers, widths, 6);

    int count = 0;
    for (size_t i = 0; i < txs->Count(); ++i) {
        Transaction* t = txs->Get(i);
        if (t->GetType() != TransactionType::Income) continue;
        ++count;
        string walletName = "-";
        Wallet* w = appController->GetWalletById(t->GetWalletId());
        if (w) walletName = w->GetName();

        string dateStr = t->GetDate().ToString();
        string desc = t->GetDescription();
        if ((int)desc.length() > 28) desc = desc.substr(0, 27) + "~";

        string data[] = {to_string(count), t->GetId(), walletName, view.FormatCurrency(static_cast<long>(t->GetAmount())), dateStr, desc};
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

    string headers[] = {"Index", "ID", "Wallet", "Amount", "Date", "Desc"};
    int widths[] = {6, 18, 20, 15, 12, 30};
    view.PrintTableHeader(headers, widths, 6);
    for (size_t i = 0; i < incomes->Count(); ++i) {
        Transaction* t = incomes->Get(i);
        Wallet* w = appController->GetWalletById(t->GetWalletId());
        string walletName = w ? w->GetName() : "-";
        string data[] = {to_string(i + 1), t->GetId(), walletName, view.FormatCurrency(static_cast<long>(t->GetAmount())), t->GetDate().ToString(), t->GetDescription()};
        view.PrintTableRow(data, widths, 6);
    }
    view.PrintTableSeparator(widths, 6);

    view.MoveToXY(5, 6 + (int)incomes->Count());
    cout << "Enter income index to edit (1-" << incomes->Count() << "): ";
    int idx = 0;
    cin >> idx;
    cin.ignore();

    if (idx < 1 || idx > static_cast<int>(incomes->Count())) {
        view.ShowError("Invalid selection.");
        PauseWithMessage("Press any key to continue...");
        delete incomes;
        return;
    }

    Transaction* target = incomes->Get(idx - 1);

    // Show current values and prompt new ones
    view.ClearScreen();
    view.PrintHeader("EDIT INCOME - DETAILS");
    Wallet* w = appController->GetWalletById(target->GetWalletId());
    string walletName = w ? w->GetName() : "-";
    view.MoveToXY(5,5);
    cout << "ID: " << target->GetId() << endl;
    cout << "Wallet: " << walletName << endl;
    cout << "Current Amount: " << view.FormatCurrency(static_cast<long>(target->GetAmount())) << endl;
    cout << "Current Date: " << target->GetDate().ToString() << endl;
    cout << "Current Description: " << target->GetDescription() << endl;

    double newAmount = InputValidator::GetValidMoney("Enter new amount: ");
    Date newDate = InputValidator::GetValidDate("Enter new date (YYYY-MM-DD): ");
    string newDesc = InputValidator::GetValidString("Enter new description: ");

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

    string headers[] = {"Index", "ID", "Wallet", "Amount", "Date", "Desc"};
    int widths[] = {6, 18, 20, 15, 12, 30};
    view.PrintTableHeader(headers, widths, 6);
    for (size_t i = 0; i < incomes->Count(); ++i) {
        Transaction* t = incomes->Get(i);
        Wallet* w = appController->GetWalletById(t->GetWalletId());
        string walletName = w ? w->GetName() : "-";
        string data[] = {to_string(i + 1), t->GetId(), walletName, view.FormatCurrency(static_cast<long>(t->GetAmount())), t->GetDate().ToString(), t->GetDescription()};
        view.PrintTableRow(data, widths, 6);
    }
    view.PrintTableSeparator(widths, 6);

    view.MoveToXY(5, 6 + (int)incomes->Count());
    cout << "Enter income index to delete (1-" << incomes->Count() << "): ";
    int idx = 0;
    cin >> idx;
    cin.ignore();

    if (idx < 1 || idx > static_cast<int>(incomes->Count())) {
        view.ShowError("Invalid selection.");
        PauseWithMessage("Press any key to continue...");
        delete incomes;
        return;
    }

    Transaction* target = incomes->Get(idx - 1);
    view.MoveToXY(5, 8 + (int)incomes->Count());
    cout << "Are you sure you want to delete this income? (Y/N): ";
    int ch = GetKeyPress();
    cout << endl;

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
