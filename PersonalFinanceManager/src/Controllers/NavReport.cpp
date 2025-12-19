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

#include "Utils/HashMap.h"
#include "Views/InputValidator.h"

#include <iostream>
#include <sstream>
#include <iomanip>

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


// Helper for aggregation results
struct CategoryTotal {
    std::string id;
    double amount;
    CategoryTotal() : id(""), amount(0.0) {}
    CategoryTotal(const std::string& i, double a) : id(i), amount(a) {}
};

static bool CompareCategoryTotal(const CategoryTotal& a, const CategoryTotal& b) {
    return a.amount > b.amount; // Descending
}

void NavigationController::HandleMonthlySummary() {
    view.ClearScreen();
    view.PrintHeader("MONTHLY SUMMARY");

    Date today = Date::GetTodayDate();
    int month = 0;
    std::cout << "Enter month (1-12) or 0 for current month: ";
    if (!(std::cin >> month)) {
        std::cin.clear(); std::cin.ignore(10000, '\n');
        view.ShowError("Invalid input.");
        PauseWithMessage("Press any key to continue...");
        return;
    }
    std::cin.ignore();
    if (month == 0) month = today.GetMonth();

    int year = 0;
    std::cout << "Enter year (e.g., 2025) or 0 for current year: ";
    if (!(std::cin >> year)) {
        std::cin.clear(); std::cin.ignore(10000, '\n');
        view.ShowError("Invalid input.");
        PauseWithMessage("Press any key to continue...");
        return;
    }
    std::cin.ignore();
    if (year == 0) year = today.GetYear();

    if (month < 1 || month > 12 || year < 1900) {
        view.ShowError("Invalid month or year.");
        PauseWithMessage("Press any key to continue...");
        return;
    }

    Date start(1, month, year);
    Date end = Date::GetEndOfMonth(month, year);

    ArrayList<Transaction*>* list = appController->GetTransactionsByDateRange(start, end);
    if (!list || list->Count() == 0) {
        view.ShowInfo("No transactions found for the selected month.");
        delete list;
        PauseWithMessage("Press any key to continue...");
        return;
    }

    double totalIncome = 0.0, totalExpense = 0.0;
    HashMap<std::string, double> expenseMap;

    for (size_t i = 0; i < list->Count(); ++i) {
        Transaction* t = list->Get(i);
        if (t->GetType() == TransactionType::Income) totalIncome += t->GetAmount();
        else {
            totalExpense += t->GetAmount();
            std::string cid = t->GetCategoryId();
            double* existing = expenseMap.Get(cid);
            if (existing) *existing += t->GetAmount();
            else expenseMap.Put(cid, t->GetAmount());
        }
    }

    view.MoveToXY(0,6);
    view.PrintText("Summary for " + std::to_string(month) + "/" + std::to_string(year));
    view.PrintText("Total Income : " + view.FormatCurrency(static_cast<long>(totalIncome)));
    view.PrintText("Total Expense: " + view.FormatCurrency(static_cast<long>(totalExpense)));
    view.PrintText("Net          : " + view.FormatCurrency(static_cast<long>(totalIncome - totalExpense)));

    if (totalExpense > 0 && expenseMap.Count() > 0) {
        // Convert map to list
        ArrayList<CategoryTotal> totals;
        ArrayList<std::string> keys = expenseMap.Keys();
        for (size_t i = 0; i < keys.Count(); ++i) {
            std::string k = keys.Get(i);
            double* v = expenseMap.Get(k);
            totals.Add(CategoryTotal(k, v ? *v : 0.0));
        }

        // Sort descending
        totals.Sort(CompareCategoryTotal);

        view.PrintText("");
        view.PrintText("Top spending categories:");
        std::string headers2[] = {"Category", "Amount", "% of Expense"};
        int widths2[] = {35, 15, 20};
        view.PrintTableHeader(headers2, widths2, 3);
        for (size_t i = 0; i < totals.Count() && i < 10; ++i) {
            Category* c = appController->GetCategoryById(totals.Get(i).id);
            std::string name = c ? c->GetName() : "Unknown";
            double amt = totals.Get(i).amount;
            double pct = (totalExpense > 0) ? ((amt / totalExpense) * 100.0) : 0.0;
            std::ostringstream pctss;
            pctss << std::fixed << std::setprecision(1) << pct;
            std::string data[] = {name, view.FormatCurrency(static_cast<long>(amt)), pctss.str() + "%"};
            view.PrintTableRow(data, widths2, 3);
        }
        view.PrintTableSeparator(widths2, 3);
    } else {
        view.ShowInfo("No expense categories to display for this month.");
    }

    delete list;
    PauseWithMessage("Press any key to continue...");
}

void NavigationController::HandleSpendingByCategory() {
    view.ClearScreen();
    view.PrintHeader("SPENDING BY CATEGORY");

    Date start = InputValidator::GetValidDate("Enter start date (YYYY-MM-DD) or 'T' for today: ");
    Date end = InputValidator::GetValidDate("Enter end date (YYYY-MM-DD) or 'T' for today: ");

    if (end < start) {
        view.ShowError("End date must be after or equal to start date.");
        PauseWithMessage("Press any key to continue...");
        return;
    }

    ArrayList<Transaction*>* list = appController->GetTransactionsByDateRange(start, end);
    if (!list || list->Count() == 0) {
        view.ShowInfo("No transactions in the selected range.");
        delete list;
        PauseWithMessage("Press any key to continue...");
        return;
    }

    HashMap<std::string, double> totalsMap;
    double totalExpense = 0.0;

    for (size_t i = 0; i < list->Count(); ++i) {
        Transaction* t = list->Get(i);
        if (t->GetType() == TransactionType::Expense) {
            std::string cid = t->GetCategoryId();
            double* ex = totalsMap.Get(cid);
            if (ex) *ex += t->GetAmount();
            else totalsMap.Put(cid, t->GetAmount());
            totalExpense += t->GetAmount();
        }
    }

    if (totalsMap.Count() == 0) {
        view.ShowInfo("No expenses found in the range.");
        delete list;
        PauseWithMessage("Press any key to continue...");
        return;
    }

    ArrayList<CategoryTotal> totals;
    ArrayList<std::string> keys = totalsMap.Keys();
    for (size_t i = 0; i < keys.Count(); ++i) {
        std::string k = keys.Get(i);
        double* v = totalsMap.Get(k);
        totals.Add(CategoryTotal(k, v ? *v : 0.0));
    }

    totals.Sort(CompareCategoryTotal);

    std::string headers[] = {"Category", "Amount", "% of Range"};
    int widths[] = {40, 20, 15};
    view.PrintTableHeader(headers, widths, 3);

    for (size_t i = 0; i < totals.Count(); ++i) {
        Category* c = appController->GetCategoryById(totals.Get(i).id);
        std::string name = (c != nullptr) ? c->GetName() : std::string("Unknown");
        double pctVal = (totalExpense > 0) ? ((totals.Get(i).amount / totalExpense) * 100.0) : 0.0;
        std::ostringstream pctss;
        pctss << std::fixed << std::setprecision(1) << pctVal;
        std::string data[] = {name, view.FormatCurrency(static_cast<long>(totals.Get(i).amount)), pctss.str() + "%"};
        view.PrintTableRow(data, widths, 3);
    }

    view.PrintTableSeparator(widths, 3);

    delete list;
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
    view.PrintText("Total Income: " + view.FormatCurrency(static_cast<long>(income)));
    view.MoveToXY(5,6);
    view.PrintText("Total Expense: " + view.FormatCurrency(static_cast<long>(expense)));
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
        double pctd = (total != 0) ? (bal / total) * 100.0 : 0.0;
        std::ostringstream pctss;
        pctss << std::fixed << std::setprecision(1) << pctd;
        std::string pct = pctss.str() + "%";
        std::string data[] = {w->GetName(), view.FormatCurrency(static_cast<long>(bal)), pct};
        view.PrintTableRow(data, widths, 3);
    }
    view.PrintTableSeparator(widths, 3);
    PauseWithMessage("Press any key to continue...");
}

