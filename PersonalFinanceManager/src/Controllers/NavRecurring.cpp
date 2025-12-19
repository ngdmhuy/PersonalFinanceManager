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
#include <sstream>

void NavigationController::ShowRecurringFlow() {
    bool inRecurring = true;
    while (inRecurring) {
        char c = menus.DisplayRecurringMenu();
        if (c == 27) break;
        switch (c) {
            case '1':
                HandleAddRecurring();
                break;
            case '2':
                HandleViewRecurring();
                break;
            case '3':
                HandleEditRecurring();
                break;
            case '4':
                HandleDeleteRecurring();
                break;
            default:
                view.ShowError("Invalid selection. Try again.");
                PauseWithMessage("Press any key to continue...");
                break;
        }
    }
}

void NavigationController::HandleAddRecurring() {
    if (!appController) { view.ShowError("Controller not available."); PauseWithMessage("Press any key to continue..."); return; }

    view.ClearScreen();
    view.PrintHeader("CREATE RECURRING TRANSACTION");

    // Choose type
    view.MoveToXY(0,4);
    view.PrintText("Select transaction type:");
    view.PrintText("1. Expense");
    view.PrintText("2. Income");
    int typeSel = InputValidator::GetValidIndex("Select (1-2) (0 to cancel): ", 1, 2, 5, 8);
    if (typeSel == 0) { view.ShowInfo("Cancelled."); PauseWithMessage("Press any key to continue..."); return; }
    TransactionType ttype = (typeSel == 1) ? TransactionType::Expense : TransactionType::Income;

    // Wallet
    ArrayList<Wallet*>* wallets = appController->GetWalletsList();
    if (!wallets || wallets->Count() == 0) { view.ShowError("No wallets available. Create a wallet first."); PauseWithMessage("Press any key to continue..."); return; }
    view.ClearScreen(); view.PrintHeader("SELECT WALLET (Recurring)");
    view.MoveToXY(5,4); view.PrintText("Available Wallets:");
    std::string wheads[] = {"Index","Wallet","Balance"}; int wwidths[] = {8,30,20};
    view.PrintTableHeader(wheads, wwidths, 3);
    for (size_t i = 0; i < wallets->Count(); ++i) {
        Wallet* w = wallets->Get(i);
        std::string data[] = {std::to_string(i+1), w->GetName(), view.FormatCurrency((long)w->GetBalance())};
        view.PrintTableRow(data, wwidths, 3);
    }
    view.PrintTableSeparator(wwidths, 3);
    int widx = InputValidator::GetValidIndex("Select wallet index (1-" + std::to_string(static_cast<int>(wallets->Count())) + ") (0 to cancel): ", 1, static_cast<int>(wallets->Count()), 5, 9 + static_cast<int>(wallets->Count()));
    if (widx == 0) { view.ShowInfo("Cancelled."); PauseWithMessage("Press any key to continue..."); return; }
    std::string walletId = wallets->Get(widx-1)->GetId();

    // Category or Source selection (optional)
    std::string catOrSrcId = "";
    if (ttype == TransactionType::Expense) {
        ArrayList<Category*>* cats = appController->GetCategoriesList();
        if (!cats || cats->Count() == 0) { view.ShowError("No categories exist. Create categories first."); PauseWithMessage("Press any key to continue..."); return; }
        view.ClearScreen(); view.PrintHeader("SELECT CATEGORY (Recurring)");
        view.MoveToXY(5,4); view.PrintText("Available Categories:");
        std::string cheads[] = {"Index","Category"}; int cwidths[] = {8,40};
        view.PrintTableHeader(cheads, cwidths, 2);
        for (size_t i = 0; i < cats->Count(); ++i) {
            Category* c = cats->Get(i);
            std::string data[] = {std::to_string(i+1), c->GetName()};
            view.PrintTableRow(data, cwidths, 2);
        }
        view.PrintTableSeparator(cwidths, 2);
        int cidx = InputValidator::GetValidIndex("Select category index (1-" + std::to_string(static_cast<int>(cats->Count())) + ") (0 to cancel): ", 1, static_cast<int>(cats->Count()), 5, 9 + static_cast<int>(cats->Count()));
        if (cidx == 0) { view.ShowInfo("Cancelled."); PauseWithMessage("Press any key to continue..."); return; }
        catOrSrcId = cats->Get(cidx-1)->GetId();
    } else {
        ArrayList<IncomeSource*>* sources = appController->GetIncomeSourcesList();
        if (!sources || sources->Count() == 0) { view.ShowError("No income sources exist. Create sources first."); PauseWithMessage("Press any key to continue..."); return; }
        view.ClearScreen(); view.PrintHeader("SELECT SOURCE (Recurring)");
        view.MoveToXY(5,4); view.PrintText("Available Sources:");
        std::string sheads[] = {"Index","Source"}; int swidths[] = {8,40};
        view.PrintTableHeader(sheads, swidths, 2);
        for (size_t i = 0; i < sources->Count(); ++i) {
            IncomeSource* s = sources->Get(i);
            std::string data[] = {std::to_string(i+1), s->GetName()};
            view.PrintTableRow(data, swidths, 2);
        }
        view.PrintTableSeparator(swidths, 2);
        int sidx = InputValidator::GetValidIndex("Select source index (1-" + std::to_string(static_cast<int>(sources->Count())) + ") (0 to cancel): ", 1, static_cast<int>(sources->Count()), 5, 9 + static_cast<int>(sources->Count()));
        if (sidx == 0) { view.ShowInfo("Cancelled."); PauseWithMessage("Press any key to continue..."); return; }
        catOrSrcId = sources->Get(sidx-1)->GetId();
    }

    // Frequency
    view.ClearScreen(); view.PrintHeader("SET FREQUENCY");
    view.PrintText("Select frequency:");
    view.PrintText("1. Daily"); view.PrintText("2. Weekly"); view.PrintText("3. Monthly"); view.PrintText("4. Yearly");
    int fsel = InputValidator::GetValidIndex("Select (1-4) (0 to cancel): ", 1, 4, 5, 9);
    if (fsel == 0) { view.ShowInfo("Cancelled."); PauseWithMessage("Press any key to continue..."); return; }
    Frequency freq = Frequency::Monthly;
    switch (fsel) { case 1: freq = Frequency::Daily; break; case 2: freq = Frequency::Weekly; break; case 3: freq = Frequency::Monthly; break; case 4: freq = Frequency::Yearly; break; }

    // Start / End dates
    view.ClearScreen(); view.PrintHeader("SET DATES");
    Date start = InputValidator::GetValidDate("Enter start date (YYYY-MM-DD) or 'T' for today: ");
    Date end = InputValidator::GetValidDate("Enter optional end date (YYYY-MM-DD) or 'T' for today (leave blank for no end): ");

    // Amount / Description
    view.ClearScreen(); view.PrintHeader("SET AMOUNT & DESCRIPTION");
    double amount = InputValidator::GetValidMoney("Enter amount: ");
    std::string desc = InputValidator::GetValidString("Enter description: ");

    // Create
    appController->AddRecurringTransaction(freq, start, end, walletId, catOrSrcId, amount, ttype, desc);
    PauseWithMessage("Press any key to continue...");
}

void NavigationController::HandleViewRecurring() {
    view.ClearScreen(); view.PrintHeader("RECURRING TRANSACTIONS");
    ArrayList<RecurringTransaction*>* list = appController->GetRecurringList();
    if (!list || list->Count() == 0) { view.ShowInfo("No recurring transactions scheduled."); PauseWithMessage("Press any key to continue..."); return; }

    std::string headers[] = {"Index","ID","Freq","Start","End","Type","Wallet","Category/Source","Amount","Desc"};
    int widths[] = {6,18,10,12,12,8,15,18,12,20};
    view.PrintTableHeader(headers, widths, 10);
    for (size_t i = 0; i < list->Count(); ++i) {
        RecurringTransaction* r = list->Get(i);
        std::string freq = EnumHelper::FrequencyToString(r->GetFrequency());
        std::string start = r->GetStartDate().ToString();
        std::string end = r->GetEndDate().IsValid() ? r->GetEndDate().ToString() : std::string("-");
        std::string type = EnumHelper::TypeToString(r->GetType());
        std::string wallet = (appController->GetWalletById(r->GetWalletId()) ? appController->GetWalletById(r->GetWalletId())->GetName() : std::string("Unknown"));
        std::string cat = (r->GetCategoryId().empty() ? std::string("-") : (appController->GetCategoryById(r->GetCategoryId()) ? appController->GetCategoryById(r->GetCategoryId())->GetName() : std::string("Unknown")));
        if (r->GetType() == TransactionType::Income) {
            if (!r->GetCategoryId().empty()) {
                IncomeSource* s = appController->GetIncomeSourceById(r->GetCategoryId());
                if (s) cat = s->GetName();
            }
        }
        std::ostringstream amt; amt << view.FormatCurrency(static_cast<long>(r->GetAmount()));
        std::string data[] = {std::to_string(static_cast<int>(i+1)), r->GetId(), freq, start, end, type, wallet, cat, amt.str(), r->GetDescription()};
        view.PrintTableRow(data, widths, 10);
    }
    view.PrintTableSeparator(widths, 10);
    PauseWithMessage("Press any key to continue...");
}

void NavigationController::HandleEditRecurring() {
    view.ClearScreen(); view.PrintHeader("EDIT RECURRING TRANSACTION");
    ArrayList<RecurringTransaction*>* list = appController->GetRecurringList();
    if (!list || list->Count() == 0) { view.ShowInfo("No recurring transactions to edit."); PauseWithMessage("Press any key to continue..."); return; }

    std::string headers[] = {"Index","ID","Desc","Amount","Type"};
    int widths[] = {6,18,40,12,8};
    view.PrintTableHeader(headers, widths, 5);
    for (size_t i = 0; i < list->Count(); ++i) {
        RecurringTransaction* r = list->Get(i);
        std::string data[] = {std::to_string(static_cast<int>(i+1)), r->GetId(), r->GetDescription(), view.FormatCurrency(static_cast<long>(r->GetAmount())), EnumHelper::TypeToString(r->GetType())};
        view.PrintTableRow(data, widths, 5);
    }
    view.PrintTableSeparator(widths, 5);

    int idx = InputValidator::GetValidIndex("Select index to edit (1-" + std::to_string(static_cast<int>(list->Count())) + ") (0 to cancel): ", 1, static_cast<int>(list->Count()), 5, 9 + static_cast<int>(list->Count()));
    if (idx == 0) { view.ShowInfo("Cancelled."); PauseWithMessage("Press any key to continue..."); return; }
    RecurringTransaction* sel = list->Get(idx-1);

    // Allow editing similar to create
    view.ClearScreen(); view.PrintHeader("EDIT RECURRING - FREQUENCY");
    view.PrintText("Current frequency: " + EnumHelper::FrequencyToString(sel->GetFrequency()));
    view.PrintText("1. Daily  2. Weekly  3. Monthly  4. Yearly");
    int fsel = InputValidator::GetValidIndex("Select new frequency (1-4) (0 to keep): ", 0, 4, 5, 9);
    Frequency freq = sel->GetFrequency();
    if (fsel != 0) switch (fsel) { case 1: freq = Frequency::Daily; break; case 2: freq = Frequency::Weekly; break; case 3: freq = Frequency::Monthly; break; case 4: freq = Frequency::Yearly; break; }

    view.ClearScreen(); view.PrintHeader("EDIT RECURRING - DATES");
    view.PrintText("Current start: " + sel->GetStartDate().ToString());
    Date start = InputValidator::GetValidDate("Enter new start date (YYYY-MM-DD) or 'T' for today (leave blank to keep): ");
    if (!start.IsValid()) start = sel->GetStartDate();
    view.PrintText("Current end: " + (sel->GetEndDate().IsValid() ? sel->GetEndDate().ToString() : std::string("(none)")));
    Date end = InputValidator::GetValidDate("Enter new end date (YYYY-MM-DD) or 'T' for today (leave blank to keep): ");
    if (!end.IsValid()) end = sel->GetEndDate();

    view.ClearScreen(); view.PrintHeader("EDIT RECURRING - AMOUNT/DESC");
    view.PrintText("Current amount: " + view.FormatCurrency(static_cast<long>(sel->GetAmount())));
    double amount = InputValidator::GetValidMoney("Enter new amount (0 to keep): ");
    if (amount <= 0) amount = sel->GetAmount();

    // Read optional description line (allow empty to keep)
    std::string descInput;
    std::cout << "Enter new description (leave blank to keep): ";
    std::getline(std::cin, descInput);
    // Trim
    size_t s = descInput.find_first_not_of(" \t\r\n");
    if (s == std::string::npos) descInput = sel->GetDescription();
    else {
        size_t e = descInput.find_last_not_of(" \t\r\n");
        descInput = descInput.substr(s, e - s + 1);
    }

    // Wallet and Category/source remain as-is for simplicity; advanced edit could support changing them
    appController->EditRecurringTransaction(sel->GetId(), freq, start, end, sel->GetWalletId(), sel->GetCategoryId(), amount, sel->GetType(), descInput);
    PauseWithMessage("Press any key to continue...");
}

void NavigationController::HandleDeleteRecurring() {
    view.ClearScreen(); view.PrintHeader("DELETE RECURRING TRANSACTION");
    ArrayList<RecurringTransaction*>* list = appController->GetRecurringList();
    if (!list || list->Count() == 0) { view.ShowInfo("No recurring transactions to delete."); PauseWithMessage("Press any key to continue..."); return; }

    std::string headers[] = {"Index","ID","Desc","Amount","Type"};
    int widths[] = {6,18,40,12,8};
    view.PrintTableHeader(headers, widths, 5);
    for (size_t i = 0; i < list->Count(); ++i) {
        RecurringTransaction* r = list->Get(i);
        std::string data[] = {std::to_string(static_cast<int>(i+1)), r->GetId(), r->GetDescription(), view.FormatCurrency(static_cast<long>(r->GetAmount())), EnumHelper::TypeToString(r->GetType())};
        view.PrintTableRow(data, widths, 5);
    }
    view.PrintTableSeparator(widths, 5);

    int idx = InputValidator::GetValidIndex("Select index to delete (1-" + std::to_string(static_cast<int>(list->Count())) + ") (0 to cancel): ", 1, static_cast<int>(list->Count()), 5, 9 + static_cast<int>(list->Count()));
    if (idx == 0) { view.ShowInfo("Cancelled."); PauseWithMessage("Press any key to continue..."); return; }

    RecurringTransaction* sel = list->Get(idx-1);
    bool ok = appController->DeleteRecurringTransaction(sel->GetId());
    if (ok) PauseWithMessage("Press any key to continue...");
}

