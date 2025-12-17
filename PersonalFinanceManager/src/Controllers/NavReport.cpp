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

void NavigationController::ShowReportsFlow() {
    bool inReportsMenu = true;

    while (inReportsMenu) {
        char c = menus.DisplayReportsMenu();

        if (c == 27) break;

        switch (c) {
            case '1':
                HandleMonthlySummary();
                break;
            case '2':
                HandleSpendingByCategory();
                break;
            case '3':
                HandleIncomeVsExpense();
                break;
            case '4':
                HandleWalletBalanceOverview();
                break;
            default:
                view.ShowError("Invalid selection. Try again.");
                PauseWithMessage("Press any key to continue...");
                break;
        }
    }
}


void NavigationController::HandleMonthlySummary() {
    view.ClearScreen();
    view.PrintHeader("MONTHLY SUMMARY");
    ArrayList<Transaction*>* txs = appController->GetTransactions();
    long count = txs ? txs->Count() : 0;
    double totalBalance = appController->GetTotalBalance();
    view.MoveToXY(5,5);
    std::cout << "Total transactions: " << count << std::endl;
    std::cout << "Total balance across wallets: " << view.FormatCurrency(static_cast<long>(totalBalance)) << std::endl;
    view.ShowInfo("Detailed monthly breakdown not implemented.");
    PauseWithMessage("Press any key to continue...");
}

void NavigationController::HandleSpendingByCategory() {
    view.ClearScreen();
    view.PrintHeader("SPENDING BY CATEGORY");
    view.ShowInfo("Feature not yet implemented.");
    PauseWithMessage("Press any key to continue...");
}

void NavigationController::HandleIncomeVsExpense() {
    view.ClearScreen();
    view.PrintHeader("INCOME VS EXPENSE");
    ArrayList<Transaction*>* txs = appController->GetTransactions();
    double income = 0.0, expense = 0.0;
    if (txs) {
        for (size_t i = 0; i < txs->Count(); ++i) {
            Transaction* t = txs->Get(i);
            if (t->GetType() == TransactionType::Income) income += t->GetAmount();
            else expense += t->GetAmount();
        }
    }
    view.MoveToXY(5,5);
    std::cout << "Total Income: " << view.FormatCurrency(static_cast<long>(income)) << std::endl;
    std::cout << "Total Expense: " << view.FormatCurrency(static_cast<long>(expense)) << std::endl;
    PauseWithMessage("Press any key to continue...");
}

void NavigationController::HandleWalletBalanceOverview() {
    view.ClearScreen();
    view.PrintHeader("WALLET BALANCE OVERVIEW");
    ArrayList<Wallet*>* wallets = appController->GetWalletsList();
    if (!wallets || wallets->Count() == 0) {
        view.ShowInfo("No wallets available.");
        PauseWithMessage("Press any key to continue...");
        return;
    }
    std::string headers[] = {"Wallet", "Balance", "% of Total"};
    int widths[] = {30, 20, 15};
    view.PrintTableHeader(headers, widths, 3);
    double total = appController->GetTotalBalance();
    for (size_t i = 0; i < wallets->Count(); ++i) {
        Wallet* w = wallets->Get(i);
        double bal = w->GetBalance();
        std::string pct = total != 0 ? std::to_string(static_cast<int>((bal / total) * 100)) + "%" : "0%";
        std::string data[] = {w->GetName(), view.FormatCurrency(static_cast<long>(bal)), pct};
        view.PrintTableRow(data, widths, 3);
    }
    view.PrintTableSeparator(widths, 3);
    PauseWithMessage("Press any key to continue...");
}

