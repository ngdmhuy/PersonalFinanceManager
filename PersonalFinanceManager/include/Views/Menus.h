#ifndef Menus_h
#define Menus_h
#include <string>
#include "Views/ConsoleView.h"

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
    static const std::string MAIN_MENU_TITLE;
    static const std::string MAIN_MENU_1;
    static const std::string MAIN_MENU_2;
    static const std::string MAIN_MENU_3;
    static const std::string MAIN_MENU_4;
    static const std::string MAIN_MENU_5;
    static const std::string MAIN_MENU_6;
    
    // Expense Menu
    static const std::string EXPENSE_MENU_TITLE;
    static const std::string EXPENSE_MENU_1;
    static const std::string EXPENSE_MENU_2;
    static const std::string EXPENSE_MENU_3;
    static const std::string EXPENSE_MENU_4;
    static const std::string EXPENSE_MENU_5;
    
    // Income Menu
    static const std::string INCOME_MENU_TITLE;
    static const std::string INCOME_MENU_1;
    static const std::string INCOME_MENU_2;
    static const std::string INCOME_MENU_3;
    static const std::string INCOME_MENU_4;
    static const std::string INCOME_MENU_5;
    
    // Wallet Menu
    static const std::string WALLET_MENU_TITLE;
    static const std::string WALLET_MENU_1;
    static const std::string WALLET_MENU_2;
    static const std::string WALLET_MENU_3;
    
    // Reports Menu
    static const std::string REPORTS_MENU_TITLE;
    static const std::string REPORTS_MENU_1;
    static const std::string REPORTS_MENU_2;
    static const std::string REPORTS_MENU_3;
    static const std::string REPORTS_MENU_4;
    
    // Add Income Form
    static const std::string ADD_INCOME_TITLE;
    static const std::string SELECT_SOURCE_PROMPT;
    
    // Add Expense Form  
    static const std::string ADD_EXPENSE_TITLE;
    static const std::string SELECT_CATEGORY_PROMPT;

    // CATEGORY & SOURCE MANAGEMENT MENUS
    char DisplayCategoryMenu();
    char DisplaySourceMenu();
    char DisplayRecurringMenu();

    static const std::string CATEGORY_MENU_TITLE;
    static const std::string CATEGORY_MENU_1;
    static const std::string CATEGORY_MENU_2;
    static const std::string CATEGORY_MENU_3;
    static const std::string CATEGORY_MENU_4;

    static const std::string SOURCE_MENU_TITLE;
    static const std::string SOURCE_MENU_1;
    static const std::string SOURCE_MENU_2;
    static const std::string SOURCE_MENU_3;
    static const std::string SOURCE_MENU_4;

    static const std::string RECURRING_MENU_TITLE;
    static const std::string RECURRING_MENU_1;
    static const std::string RECURRING_MENU_2;
    static const std::string RECURRING_MENU_3;
    static const std::string RECURRING_MENU_4;
};

#endif // !Menus_h
