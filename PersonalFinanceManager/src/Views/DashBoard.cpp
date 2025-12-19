#include "Views/DashBoard.h"
#include "Views/Menus.h"
#include "Utils/PlatformUtils.h"
#include "Models/Wallet.h"
#include "Models/Transaction.h"
#include "Controllers/AppController.h"
 

char Dashboard::Display() {
    // Precompute table width for wallet list so header/footer match
    int dashboardTableWidths[] = {25, 25, 15};
    int dashboardNumCols = 3;
    int dashboardTableWidth = 1; for (int i = 0; i < dashboardNumCols; ++i) dashboardTableWidth += dashboardTableWidths[i] + 1;

    view.ClearScreen();
    view.PrintHeader("PERSONAL FINANCE MANAGER - DASHBOARD", dashboardTableWidth);

    // Prefer live in-memory data from AppController if bound
    ArrayList<Wallet*>* wallets = nullptr;
    ArrayList<Transaction*>* transactions = nullptr;

    if (appController) {
        wallets = appController->GetWalletsList();
        transactions = appController->GetTransactions();
    }

    // Compute total balance using AppController if available
    double totalBalance = (appController) ? appController->GetTotalBalance() : 0.0;

    view.MoveToXY(5, 5);
    if (totalBalance < 0) view.SetColor(ConsoleView::COLOR_ERROR); // Red color for negative balance
    else view.SetColor(ConsoleView::COLOR_SUCCESS); // Green color for positive balance

    view.PrintText("Total Balance: " + view.FormatCurrency(static_cast<long>(totalBalance)));
    view.ResetColor();

    // Show helpful messages based on in-memory lists
    if (!appController) {
        view.ShowInfo("Dashboard not bound to AppController. Data may be out of date.");
    } else if (!wallets || wallets->Count() == 0) {
        view.ShowInfo("No wallets found. Create a wallet from Wallet Menu.");
    }

    if (appController && (!transactions || transactions->Count() == 0)) {
        view.ShowInfo("No transactions recorded yet.");
    }

    // Display wallet list
    std::string headers[] = {"Wallet Name", "Balance", "Transactions"};
    int widths[] = {25, 25, 15};
    int numCols = 3;

    view.PrintTableHeader(headers, widths, numCols);
    if (!appController || !wallets || wallets->Count() == 0) {
        std::string row[] = {"No wallets found", "", ""};
        view.PrintTableRow(row, widths, numCols);
    } else {
        for (size_t i = 0; i < wallets->Count(); ++i) {
            Wallet* w = wallets->Get(i);
            // Count transactions for this wallet
            int txCount = 0;
            if (transactions) {
                for (size_t j = 0; j < transactions->Count(); ++j) {
                    Transaction* t = transactions->Get(j);
                    if (t->GetWalletId() == w->GetId()) ++txCount;
                }
            }

            std::string name = w->GetName();
            std::string balance = view.FormatCurrency(static_cast<long>(w->GetBalance()));
            std::string txs = std::to_string(txCount);
            std::string row[] = {name, balance, txs};
            view.PrintTableRow(row, widths, numCols);
        }
    }
    view.PrintTableSeparator(widths,numCols);

    view.PrintShortcutFooter("[M] Main Menu | [ESC] Exit", "Dashboard");

    // Wait for user input to navigate
    return GetKeyPress();
}
