#ifndef NavigationController_h
#define NavigationController_h

#include <string>

#include "Views/DashBoard.h"
#include "Views/Menus.h"
#include "Views/ConsoleView.h"
#include "Views/InputValidator.h"

// Forward Declaration
class AppController;

class NavigationController {
public:
    // Constructor
    NavigationController(AppController* app);
    ~NavigationController();

    // Start main navigation loop (blocks until exit)
    void Run();

private:
    AppController* appController;
    Dashboard dashboard;
    Menus menus;
    ConsoleView view;

    bool running;

    // Menu / flow handlers
    void HandleMainMenuChoice(char c);
    void ShowIncomeFlow();
    void ShowExpenseFlow();
    void ShowWalletFlow();
    void ShowReportsFlow();

    // Income handlers
    void HandleAddIncome();
    void HandleViewIncome();
    void HandleEditIncome();
    void HandleDeleteIncome();

    // Expense handlers
    void HandleAddExpense();
    void HandleViewExpenses();
    void HandleEditExpense();
    void HandleDeleteExpense();

    // Wallet handlers
    void HandleCreateWallet();
    void HandleViewWallets();
    void HandleDeleteWallet();

    // Category handlers
    void ShowCategoryFlow();
    void HandleCreateCategory();
    void HandleViewCategories();
    void HandleEditCategory();
    void HandleDeleteCategory();

    // Source handlers
    void ShowSourceFlow();
    void HandleCreateSource();
    void HandleViewSources();
    void HandleEditSource();
    void HandleDeleteSource();

    // Reports handlers
    void HandleMonthlySummary();
    void HandleSpendingByCategory();
    void HandleIncomeVsExpense();
    void HandleWalletBalanceOverview();

    // Recurring transaction handlers
    void ShowRecurringFlow();
    void HandleAddRecurring();
    void HandleViewRecurring();
    void HandleEditRecurring();
    void HandleDeleteRecurring();

    // Helper: show message and wait for key
    void PauseWithMessage(const std::string& msg);
};

#endif /* NavigationController_h */
