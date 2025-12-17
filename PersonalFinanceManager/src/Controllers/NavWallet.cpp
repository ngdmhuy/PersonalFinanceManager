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
    string name = InputValidator::GetValidString("Enter wallet name: ");
    double initial = InputValidator::GetValidMoney("Enter initial balance: ");

    try {
        appController->AddWallet(name, initial);
        view.ShowSuccess("Wallet created successfully.");
    } catch (const exception& e) {
        view.ShowError(string("Error creating wallet: ") + e.what());
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

    string headers[] = {"Index", "Wallet Name", "Balance"};
    int widths[] = {10, 30, 20};
    view.PrintTableHeader(headers, widths, 3);
    for (size_t i = 0; i < wallets->Count(); ++i) {
        Wallet* w = wallets->Get(i);
        string data[] = {to_string(i + 1), w->GetName(), view.FormatCurrency(static_cast<long>(w->GetBalance()))};
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

    string headers[] = {"Index", "Wallet Name", "Balance"};
    int widths[] = {10, 30, 20};
    view.PrintTableHeader(headers, widths, 3);
    for (size_t i = 0; i < wallets->Count(); ++i) {
        Wallet* w = wallets->Get(i);
        string data[] = {to_string(i + 1), w->GetName(), view.FormatCurrency(static_cast<long>(w->GetBalance()))};
        view.PrintTableRow(data, widths, 3);
    }
    view.PrintTableSeparator(widths, 3);

    view.MoveToXY(5, 6 + (int)wallets->Count());
    cout << "Enter wallet index to delete (1-" << wallets->Count() << "): ";
    int idx = 0;
    cin >> idx;
    cin.ignore();

    if (idx < 1 || idx > static_cast<int>(wallets->Count())) {
        view.ShowError("Invalid wallet selection.");
        PauseWithMessage("Press any key to continue...");
        return;
    }

    Wallet* target = wallets->Get(idx - 1);
    view.MoveToXY(5, 8 + (int)wallets->Count());
    cout << "Are you sure you want to delete wallet '" << target->GetName() << "'? (Y/N): ";
    int ch = GetKeyPress();
    cout << endl;

    if (ch != 'y' && ch != 'Y') {
        view.ShowInfo("Deletion cancelled.");
        PauseWithMessage("Press any key to continue...");
        return;
    }

    bool ok = appController->DeleteWallet(target->GetId());
    if (ok) {
        view.ShowSuccess("Wallet deleted successfully.");
    } else {
        view.ShowError("Failed to delete wallet. Check for dependent transactions or recurring setups.");
    }
    PauseWithMessage("Press any key to continue...");
}
