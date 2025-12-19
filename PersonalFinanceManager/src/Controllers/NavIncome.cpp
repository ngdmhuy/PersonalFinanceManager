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
            case '5':
                ShowSourceFlow();
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
    view.PrintText("Available Wallets:");
    
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
    int walletIdx = InputValidator::GetValidIndex("Select index (1-" + std::to_string(static_cast<int>(wallets->Count())) + ") (0 to cancel): ", 1, static_cast<int>(wallets->Count()), 5, 9 + static_cast<int>(wallets->Count()));
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
    Date date = InputValidator::GetValidDate("Enter date (YYYY-MM-DD) or 'T' for today: ");

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
    view.PrintText("Available Income Sources:");

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
    int srcIdx = InputValidator::GetValidIndex("Select index (1-" + std::to_string(static_cast<int>(sources->Count())) + ") (0 to cancel): ", 1, static_cast<int>(sources->Count()), 5, 9 + static_cast<int>(sources->Count()));
    if (srcIdx == 0) { view.ShowInfo("Selection cancelled."); PauseWithMessage("Press any key to continue..."); return; }
    IncomeSource* selectedSource = sources->Get(srcIdx - 1);
    std::string sourceId = selectedSource->GetId();

    // Step 6: Add transaction to AppController
    try {
        appController->AddTransaction(amount, walletId, sourceId, TransactionType::Income, date, description);
        
        view.ClearScreen();
        view.PrintHeader("INCOME ADDED SUCCESSFULLY");
        view.MoveToXY(5, 5);
        
        view.MoveToXY(5, 7);
        view.PrintText("Wallet: " + selectedWallet->GetName());
        view.MoveToXY(5, 8);
        view.PrintText("Amount: " + view.FormatCurrency(static_cast<long>(amount)));
        view.MoveToXY(5, 9);
        view.PrintText("Source: " + selectedSource->GetName());
        view.MoveToXY(5, 10);
        view.PrintText("Date: " + date.ToString());
        view.MoveToXY(5, 11);
        view.PrintText("Description: " + description);
        
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
    int idx = InputValidator::GetValidIndex("Select index (1-" + std::to_string(static_cast<int>(incomes->Count())) + ") (0 to cancel): ", 1, static_cast<int>(incomes->Count()), 5, 9 + static_cast<int>(incomes->Count()));
    if (idx == 0) { view.ShowInfo("Selection cancelled."); delete incomes; PauseWithMessage("Press any key to continue..."); return; }
    Transaction* target = incomes->Get(idx - 1);

    // Show current values and prompt new ones
    view.ClearScreen();
    view.PrintHeader("EDIT INCOME - DETAILS");
    Wallet* w = appController->GetWalletById(target->GetWalletId());
    std::string walletName = w ? w->GetName() : "-";
    view.MoveToXY(5,5);
    view.PrintText("ID: " + target->GetId());
    view.PrintText("Wallet: " + walletName);
    view.PrintText("Current Amount: " + view.FormatCurrency(static_cast<long>(target->GetAmount())));
    view.PrintText("Current Date: " + target->GetDate().ToString());
    view.PrintText("Current Description: " + target->GetDescription());

    double newAmount = InputValidator::GetValidMoney("Enter new amount: ");
    Date newDate = InputValidator::GetValidDate("Enter new date (YYYY-MM-DD) or 'T' for today: ");
    std::string newDesc = InputValidator::GetValidString("Enter new description: ");

    appController->EditTransaction(target->GetId(), newAmount, newDate, newDesc);

    delete incomes;
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
    int idx = InputValidator::GetValidIndex("Select index (1-" + std::to_string(static_cast<int>(incomes->Count())) + ") (0 to cancel): ", 1, static_cast<int>(incomes->Count()), 5, 9 + static_cast<int>(incomes->Count()));
    if (idx == 0) { view.ShowInfo("Selection cancelled."); delete incomes; PauseWithMessage("Press any key to continue..."); return; }
    Transaction* target = incomes->Get(idx - 1);
    view.MoveToXY(5, 11 + (int)incomes->Count());
    std::cout << "Are you sure you want to delete this income? (Y/N): ";
    int ch = GetKeyPress();
    // Emit newline and update cursor tracking
    view.PrintText("");

    if (ch != 'y' && ch != 'Y') {
        view.ShowInfo("Deletion cancelled.");
        PauseWithMessage("Press any key to continue...");
        return;
    }

    appController->DeleteTransaction(target->GetId());
    delete incomes;
    PauseWithMessage("Press any key to continue...");
}

// ===== SOURCE MANAGEMENT =====
void NavigationController::ShowSourceFlow() {
    bool inSrc = true;
    while (inSrc) {
        char c = menus.DisplaySourceMenu();
        if (c == 27) break;
        switch (c) {
            case '1': HandleCreateSource(); break;
            case '2': HandleViewSources(); break;
            case '3': HandleEditSource(); break;
            case '4': HandleDeleteSource(); break;
            default: view.ShowError("Invalid selection. Try again."); PauseWithMessage("Press any key to continue..."); break;
        }
    }
}

void NavigationController::HandleCreateSource() {
    view.ClearScreen();
    view.PrintHeader("CREATE SOURCE");
    std::string name = InputValidator::GetValidString("Enter source name (0 to cancel): ");
    if (name == "0") { view.ShowInfo("Source creation cancelled."); PauseWithMessage("Press any key to continue..."); return; }
    appController->AddIncomeSource(name);
    PauseWithMessage("Press any key to continue...");
}

void NavigationController::HandleViewSources() {
    view.ClearScreen();
    view.PrintHeader("SOURCES");
    ArrayList<IncomeSource*>* srcs = appController->GetIncomeSourcesList();
    if (!srcs || srcs->Count() == 0) { view.ShowInfo("No sources available."); PauseWithMessage("Press any key to continue..."); return; }

    std::string headers[] = {"Index", "ID", "Name"};
    int widths[] = {8, 20, 40};
    view.PrintTableHeader(headers, widths, 3);
    for (size_t i=0;i<srcs->Count();++i) {
        IncomeSource* s = srcs->Get(i);
        std::string data[] = {std::to_string((int)i+1), s->GetId(), s->GetName()};
        view.PrintTableRow(data, widths, 3);
    }
    view.PrintTableSeparator(widths, 3);
    PauseWithMessage("Press any key to continue...");
}

void NavigationController::HandleEditSource() {
    view.ClearScreen();
    view.PrintHeader("EDIT SOURCE");
    ArrayList<IncomeSource*>* srcs = appController->GetIncomeSourcesList();
    if (!srcs || srcs->Count() == 0) { view.ShowInfo("No sources to edit."); PauseWithMessage("Press any key to continue..."); return; }

    std::string headers[] = {"Index", "ID", "Name"};
    int widths[] = {8, 20, 40};
    view.PrintTableHeader(headers, widths, 3);
    for (size_t i=0;i<srcs->Count();++i) {
        IncomeSource* s = srcs->Get(i);
        std::string data[] = {std::to_string((int)i+1), s->GetId(), s->GetName()};
        view.PrintTableRow(data, widths, 3);
    }
    view.PrintTableSeparator(widths, 3);

    view.MoveToXY(5, 9 + (int)srcs->Count());
    int idx = InputValidator::GetValidIndex("Select index (1-" + std::to_string((int)srcs->Count()) + ") (0 to cancel): ", 1, (int)srcs->Count(), 5, 9 + (int)srcs->Count());
    if (idx == 0) { view.ShowInfo("Selection cancelled."); PauseWithMessage("Press any key to continue..."); return; }

    IncomeSource* target = srcs->Get(idx-1);
    std::string newName = InputValidator::GetValidString("Enter new name: ");
    appController->EditIncomeSource(target->GetId(), newName);
    PauseWithMessage("Press any key to continue...");
}

void NavigationController::HandleDeleteSource() {
    view.ClearScreen();
    view.PrintHeader("DELETE SOURCE");
    ArrayList<IncomeSource*>* srcs = appController->GetIncomeSourcesList();
    if (!srcs || srcs->Count() == 0) { view.ShowInfo("No sources to delete."); PauseWithMessage("Press any key to continue..."); return; }

    std::string headers[] = {"Index", "ID", "Name"};
    int widths[] = {8, 20, 40};
    view.PrintTableHeader(headers, widths, 3);
    for (size_t i=0;i<srcs->Count();++i) {
        IncomeSource* s = srcs->Get(i);
        std::string data[] = {std::to_string((int)i+1), s->GetId(), s->GetName()};
        view.PrintTableRow(data, widths, 3);
    }
    view.PrintTableSeparator(widths, 3);

    view.MoveToXY(5, 9 + (int)srcs->Count());
    int idx = InputValidator::GetValidIndex("Select index to delete (1-" + std::to_string((int)srcs->Count()) + ") (0 to cancel): ", 1, (int)srcs->Count(), 5, 9 + (int)srcs->Count());
    if (idx == 0) { view.ShowInfo("Deletion cancelled."); PauseWithMessage("Press any key to continue..."); return; }

    IncomeSource* target = srcs->Get(idx-1);
    view.MoveToXY(5, 11 + (int)srcs->Count());
    std::cout << "Are you sure you want to delete source '" << target->GetName() << "'? (Y/N): ";
    int ch = GetKeyPress();
    view.PrintText("");
    if (ch != 'y' && ch != 'Y') { view.ShowInfo("Deletion cancelled."); PauseWithMessage("Press any key to continue..."); return; }

    appController->DeleteIncomeSource(target->GetId());
    PauseWithMessage("Press any key to continue...");
}
