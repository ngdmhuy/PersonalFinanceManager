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

void NavigationController::ShowWalletFlow() {
    bool inWalletMenu = true;

    while (inWalletMenu) {
        char c = menus.DisplayWalletMenu();

        if (c == 27) {
            break;
        }

        switch (c) {
            case '1':
                HandleCreateWallet();
                break;
            case '2':
                HandleViewWallets();
                break;
            case '3':
                HandleDeleteWallet();
                break;
            default:
                view.ShowError("Invalid selection. Try again.");
                PauseWithMessage("Press any key to continue...");
                break;
        }
    }
}

void NavigationController::HandleCreateWallet() {
    view.ClearScreen();
    view.PrintHeader("CREATE WALLET");
    std::string name = InputValidator::GetValidString("Enter wallet name (0 to cancel): ");
    if (name == "0") {view.ShowInfo("Wallet creation cancelled."); PauseWithMessage("Press any key to continue..."); return;}
    double initial = InputValidator::GetValidMoney("Enter initial balance: ");

    try {
        appController->AddWallet(name, initial);
    } catch (const std::exception& e) {
        view.ShowError(std::string("Error creating wallet: ") + e.what());
    }
    PauseWithMessage("Press any key to continue...");
}

void NavigationController::HandleViewWallets() {
    view.ClearScreen();
    view.PrintHeader("WALLETS");
    ArrayList<Wallet*>* wallets = appController->GetWalletsList();
    if (!wallets || wallets->Count() == 0) {
        view.ShowInfo("No wallets available.");
        PauseWithMessage("Press any key to continue...");
        return;
    }

    std::string headers[] = {"Index", "Wallet Name", "Balance"};
    int widths[] = {10, 30, 20};
    view.PrintTableHeader(headers, widths, 3);
    for (size_t i = 0; i < wallets->Count(); ++i) {
        Wallet* w = wallets->Get(i);
        std::string data[] = {std::to_string(i + 1), w->GetName(), view.FormatCurrency(static_cast<long>(w->GetBalance()))};
        view.PrintTableRow(data, widths, 3);
    }
    view.PrintTableSeparator(widths, 3);
    PauseWithMessage("Press any key to continue...");
}

void NavigationController::HandleDeleteWallet() {
    if (!appController) {
        view.ShowError("Controller not available.");
        PauseWithMessage("Press any key to continue...");
        return;
    }

    view.ClearScreen();
    view.PrintHeader("DELETE WALLET");

    ArrayList<Wallet*>* wallets = appController->GetWalletsList();
    if (!wallets || wallets->Count() == 0) {
        view.ShowInfo("No wallets available.");
        PauseWithMessage("Press any key to continue...");
        return;
    }

    std::string headers[] = {"Index", "Wallet Name", "Balance"};
    int widths[] = {10, 30, 20};
    view.PrintTableHeader(headers, widths, 3);
    for (size_t i = 0; i < wallets->Count(); ++i) {
        Wallet* w = wallets->Get(i);
        std::string data[] = {std::to_string(i + 1), w->GetName(), view.FormatCurrency(static_cast<long>(w->GetBalance()))};
        view.PrintTableRow(data, widths, 3);
    }
    view.PrintTableSeparator(widths, 3);

    int idx = InputValidator::GetValidIndex("Select index (1-" + std::to_string(static_cast<int>(wallets->Count())) + ") (0 to cancel): ", 1, static_cast<int>(wallets->Count()), 5, 9 + static_cast<int>(wallets->Count()));
    if (idx == 0) { view.ShowInfo("Selection cancelled."); PauseWithMessage("Press any key to continue..."); return; }

    Wallet* target = wallets->Get(idx - 1);
    view.MoveToXY(5, 11 + (int)wallets->Count());
    std::cout << "Are you sure you want to delete wallet '" << target->GetName() << "'? (Y/N): ";
    int ch = GetKeyPress();
    // Emit newline and update internal cursor tracking
    view.PrintText("");

    if (ch != 'y' && ch != 'Y') {
        view.ShowInfo("Deletion cancelled.");
        PauseWithMessage("Press any key to continue...");
        return;
    }

    appController->DeleteWallet(target->GetId());
    PauseWithMessage("Press any key to continue...");
}
