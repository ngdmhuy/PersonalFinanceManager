#include "Views/Menus.h"
#include "Utils/PlatformUtils.h" // Replaces conio.h
#include <iostream>

using namespace std;

//CONSTRUCTOR
Menus::Menus() {}

//MAIN MENU IMPLEMENTATION
char Menus::DisplayMainMenu() {
    view.ClearScreen();
    view.PrintHeader(MAIN_MENU_TITLE);
    view.PrintBox(8,5,35,8);

    view.MoveToXY(10, 6);
    cout << MAIN_MENU_1 << endl;
    view.MoveToXY(10, 7);
    cout << MAIN_MENU_2 << endl;
    view.MoveToXY(10, 8);
    cout << MAIN_MENU_3 << endl;
    view.MoveToXY(10, 9);
    cout << MAIN_MENU_4 << endl;
    view.MoveToXY(10, 10);
    cout << MAIN_MENU_5 << endl;
    view.MoveToXY(10, 11);
    cout << MAIN_MENU_6 << endl;

    view.PrintShortcutFooter("[1-6] Select | [ESC] Dashboard", "Main Menu");
    
    // CROSS-PLATFORM FIX
    return GetKeyPress();
}

//EXPENSE MENU IMPLEMENTATION
char Menus::DisplayExpenseMenu() {
    view.ClearScreen();
    view.PrintHeader(EXPENSE_MENU_TITLE);
    view.PrintBox(8,5,30,7);

    view.MoveToXY(10, 6);
    cout << EXPENSE_MENU_1 << endl;
    view.MoveToXY(10, 7);
    cout << EXPENSE_MENU_2 << endl;
    view.MoveToXY(10, 8);
    cout << EXPENSE_MENU_3 << endl;
    view.MoveToXY(10, 9);
    cout << EXPENSE_MENU_4 << endl;
    view.MoveToXY(10, 10);
    cout << EXPENSE_MENU_5 << endl;

    view.PrintShortcutFooter("[1-5] Select | [ESC] Back", "Expense Menu");
    
    return GetKeyPress();
}

//INCOME MENU IMPLEMENTATION
char Menus::DisplayIncomeMenu() {
    view.ClearScreen();
    view.PrintHeader(INCOME_MENU_TITLE);
    view.PrintBox(8,5,30,7);

    view.MoveToXY(10, 6);
    cout << INCOME_MENU_1 << endl;
    view.MoveToXY(10, 7);
    cout << INCOME_MENU_2 << endl;
    view.MoveToXY(10, 8);
    cout << INCOME_MENU_3 << endl;
    view.MoveToXY(10, 9);
    cout << INCOME_MENU_4 << endl;
    view.MoveToXY(10, 10);
    cout << INCOME_MENU_5 << endl;

    view.PrintShortcutFooter("[1-5] Select | [ESC] Back", "Income Menu");
    
    return GetKeyPress();
}

//WALLET MENU IMPLEMENTATION
char Menus::DisplayWalletMenu() {
    view.ClearScreen();
    view.PrintHeader(WALLET_MENU_TITLE);
    view.PrintBox(8,5,30,5);

    view.MoveToXY(10, 6);
    cout << WALLET_MENU_1 << endl;
    view.MoveToXY(10, 7);
    cout << WALLET_MENU_2 << endl;
    view.MoveToXY(10, 8);
    cout << WALLET_MENU_3 << endl;

    view.PrintShortcutFooter("[1-3] Select | [ESC] Back", "Wallet Menu");
    
    return GetKeyPress();
}

//REPORTS MENU IMPLEMENTATION
char Menus::DisplayReportsMenu() {
    view.ClearScreen();
    view.PrintHeader(REPORTS_MENU_TITLE);
    view.PrintBox(8,5,30,6);

    view.MoveToXY(10, 6);
    cout << REPORTS_MENU_1 << endl;
    view.MoveToXY(10, 7);
    cout << REPORTS_MENU_2 << endl;
    view.MoveToXY(10, 8);
    cout << REPORTS_MENU_3 << endl;
    view.MoveToXY(10, 9);
    cout << REPORTS_MENU_4 << endl;

    view.PrintShortcutFooter("[1-4] Select | [ESC] Back", "Reports Menu");
    
    return GetKeyPress();
}

//CATEGORY MENU IMPLEMENTATION
char Menus::DisplayCategoryMenu() {
    view.ClearScreen();
    view.PrintHeader(CATEGORY_MENU_TITLE);
    view.PrintBox(8,5,30,6);

    view.MoveToXY(10, 6);
    cout << CATEGORY_MENU_1 << endl;
    view.MoveToXY(10, 7);
    cout << CATEGORY_MENU_2 << endl;
    view.MoveToXY(10, 8);
    cout << CATEGORY_MENU_3 << endl;
    view.MoveToXY(10, 9);
    cout << CATEGORY_MENU_4 << endl;

    view.PrintShortcutFooter("[1-4] Select | [ESC] Back", "Categories");
    return GetKeyPress();
}

//SOURCE MENU IMPLEMENTATION
char Menus::DisplaySourceMenu() {
    view.ClearScreen();
    view.PrintHeader(SOURCE_MENU_TITLE);
    view.PrintBox(8,5,30,6);

    view.MoveToXY(10, 6);
    cout << SOURCE_MENU_1 << endl;
    view.MoveToXY(10, 7);
    cout << SOURCE_MENU_2 << endl;
    view.MoveToXY(10, 8);
    cout << SOURCE_MENU_3 << endl;
    view.MoveToXY(10, 9);
    cout << SOURCE_MENU_4 << endl;

    view.PrintShortcutFooter("[1-4] Select | [ESC] Back", "Sources");
    return GetKeyPress();
}

// RECURRING MENU IMPLEMENTATION
char Menus::DisplayRecurringMenu() {
    view.ClearScreen();
    view.PrintHeader(RECURRING_MENU_TITLE);
    view.PrintBox(8,5,40,6);

    view.MoveToXY(10, 6);
    cout << RECURRING_MENU_1 << endl;
    view.MoveToXY(10, 7);
    cout << RECURRING_MENU_2 << endl;
    view.MoveToXY(10, 8);
    cout << RECURRING_MENU_3 << endl;
    view.MoveToXY(10, 9);
    cout << RECURRING_MENU_4 << endl;

    view.PrintShortcutFooter("[1-4] Select | [ESC] Back", "Recurring Transactions");
    return GetKeyPress();
}

//STRING CONSTANTS
// Main Menu
const string Menus::MAIN_MENU_TITLE = "PERSONAL FINANCE MANAGER - MAIN MENU";
const string Menus::MAIN_MENU_1 = "1. Income Management";
const string Menus::MAIN_MENU_2 = "2. Expense Management";
const string Menus::MAIN_MENU_3 = "3. View Reports & Statistics";
const string Menus::MAIN_MENU_4 = "4. Manage Wallets";
const string Menus::MAIN_MENU_5 = "5. Manage Recurring Transactions";
const string Menus::MAIN_MENU_6 = "6. Exit";

// Recurring Menu
const string Menus::RECURRING_MENU_TITLE = "RECURRING TRANSACTIONS";
const string Menus::RECURRING_MENU_1 = "1. Create Recurring Transaction";
const string Menus::RECURRING_MENU_2 = "2. View Recurring Transactions";
const string Menus::RECURRING_MENU_3 = "3. Edit Recurring Transaction";
const string Menus::RECURRING_MENU_4 = "4. Delete Recurring Transaction"; 

// Expense Menu
const string Menus::EXPENSE_MENU_TITLE = "EXPENSE MANAGEMENT";
const string Menus::EXPENSE_MENU_1 = "1. Add Expense";
const string Menus::EXPENSE_MENU_2 = "2. View Expenses";
const string Menus::EXPENSE_MENU_3 = "3. Edit Expense";
const string Menus::EXPENSE_MENU_4 = "4. Delete Expense";
const string Menus::EXPENSE_MENU_5 = "5. Manage Categories";

// Income Menu
const string Menus::INCOME_MENU_TITLE = "INCOME MANAGEMENT";
const string Menus::INCOME_MENU_1 = "1. Add Income";
const string Menus::INCOME_MENU_2 = "2. View Income";
const string Menus::INCOME_MENU_3 = "3. Edit Income";
const string Menus::INCOME_MENU_4 = "4. Delete Income";
const string Menus::INCOME_MENU_5 = "5. Manage Sources";

// Wallet Menu
const string Menus::WALLET_MENU_TITLE = "WALLET MANAGEMENT";
const string Menus::WALLET_MENU_1 = "1. Create Wallet";
const string Menus::WALLET_MENU_2 = "2. View Wallets";
const string Menus::WALLET_MENU_3 = "3. Delete Wallet";

// Reports Menu
const string Menus::REPORTS_MENU_TITLE = "REPORTS & STATISTICS";
const string Menus::REPORTS_MENU_1 = "1. Monthly Summary";
const string Menus::REPORTS_MENU_2 = "2. Spending by Category";
const string Menus::REPORTS_MENU_3 = "3. Income vs Expense";
const string Menus::REPORTS_MENU_4 = "4. Wallet Balance Overview";

// Add Income Form
const string Menus::ADD_INCOME_TITLE = "=== ADD INCOME ===";
const string Menus::SELECT_SOURCE_PROMPT = "Enter Income Source ID: ";

// Add Expense Form
const string Menus::ADD_EXPENSE_TITLE = "=== ADD EXPENSE ===";
const string Menus::SELECT_CATEGORY_PROMPT = "Enter Category ID: ";

// Category Management
const string Menus::CATEGORY_MENU_TITLE = "MANAGE CATEGORIES";
const string Menus::CATEGORY_MENU_1 = "1. Create Category";
const string Menus::CATEGORY_MENU_2 = "2. View Categories";
const string Menus::CATEGORY_MENU_3 = "3. Edit Category";
const string Menus::CATEGORY_MENU_4 = "4. Delete Category";

// Source Management
const string Menus::SOURCE_MENU_TITLE = "MANAGE INCOME SOURCES";
const string Menus::SOURCE_MENU_1 = "1. Create Source";
const string Menus::SOURCE_MENU_2 = "2. View Sources";
const string Menus::SOURCE_MENU_3 = "3. Edit Source";
const string Menus::SOURCE_MENU_4 = "4. Delete Source";
