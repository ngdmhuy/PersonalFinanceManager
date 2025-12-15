#ifndef MENUS_H
#define MENUS_H
#include <string>
#include "../Views/ConsoleView.h"

using namespace std;

class Menus {
private:
    ConsoleView view;

public:
    Menus();

    //MAIN MENU
    /// @brief Displays main menu and returns user choice (1-5)
    /// @return char User's menu selection
    char DisplayMainMenu();

    //EXPENSE MENU
    /// @brief Displays expense management menu
    /// @return char User's submenu selection
    char DisplayExpenseMenu();

    //NCOME MENU
    /// @brief Displays income management menu
    /// @return char User's submenu selection
    char DisplayIncomeMenu();

    //WALLET MENU
    /// @brief Displays wallet management menu
    /// @return char User's submenu selection
    char DisplayWalletMenu();

    //REPORTS MENU
    /// @brief Displays reports and analytics menu
    /// @return char User's submenu selection
    char DisplayReportsMenu();

    //MENU TEXT CONSTANTS
    // Main Dashboard Menu
    static const string MAIN_MENU_TITLE;
    static const string MAIN_MENU_1;
    static const string MAIN_MENU_2;
    static const string MAIN_MENU_3;
    static const string MAIN_MENU_4;
    static const string MAIN_MENU_5;
    
    // Expense Menu
    static const string EXPENSE_MENU_TITLE;
    static const string EXPENSE_MENU_1;
    static const string EXPENSE_MENU_2;
    static const string EXPENSE_MENU_3;
    static const string EXPENSE_MENU_4;
    
    // Income Menu
    static const string INCOME_MENU_TITLE;
    static const string INCOME_MENU_1;
    static const string INCOME_MENU_2;
    static const string INCOME_MENU_3;
    static const string INCOME_MENU_4;
    
    // Wallet Menu
    static const string WALLET_MENU_TITLE;
    static const string WALLET_MENU_1;
    static const string WALLET_MENU_2;
    static const string WALLET_MENU_3;
    
    // Reports Menu
    static const string REPORTS_MENU_TITLE;
    static const string REPORTS_MENU_1;
    static const string REPORTS_MENU_2;
    static const string REPORTS_MENU_3;
    static const string REPORTS_MENU_4;
    
    // Add Income Form
    static const string ADD_INCOME_TITLE;
    static const string SELECT_SOURCE_PROMPT;
    
    // Add Expense Form  
    static const string ADD_EXPENSE_TITLE;
    static const string SELECT_CATEGORY_PROMPT;
};

#endif
