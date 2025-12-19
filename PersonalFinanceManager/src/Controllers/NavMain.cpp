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


NavigationController::NavigationController(AppController* app) : appController(app), running(false) {
    // Bind dashboard to app controller so dashboard uses live in-memory data
    dashboard.SetAppController(appController);
}

NavigationController::~NavigationController() {}


void NavigationController::Run() {
    running = true;

    while (running) {
        // Show dashboard (uses appController internally)
        while (true) {
            char choice = dashboard.Display();
            if (choice == 27) {
                running = false;
                break;
            }
            if (choice == 'm' || choice == 'M') break;
            view.ShowError("Invalid selection!!");
            PauseWithMessage("Press any key to continue...   ");
        }
        // Keep showing the Main Menu until user chooses to return to Dashboard (ESC)
        while (running) {
            char choice = menus.DisplayMainMenu();

            if (choice == 27) { // ESC => Dashboard
                break;
            }

            // Only accept valid menu commands; show error and re-display otherwise
            switch (choice) {
                case '1':
                case '2':
                case '3':
                case '4':
                case '5':
                case '6':
                    HandleMainMenuChoice(choice);
                    break;
                default:
                    view.ShowError("Invalid selection. Try again.");
                    PauseWithMessage("Press any key to continue...                 ");
                    break;
            }
        }
    }
}

void NavigationController::HandleMainMenuChoice(char c) {
    switch (c) {
        case '1':
            ShowIncomeFlow();
            break;
        case '2':
            ShowExpenseFlow();
            break;
        case '3':
            ShowReportsFlow();
            break;
        case '4':
            ShowWalletFlow();
            break;
        case '5':
            ShowRecurringFlow();
            break;
        case '6':
            running = false;
            break;
        default:
            view.ShowError("Invalid selection. Try again.");
            break;
    }
}

void NavigationController::PauseWithMessage(const std::string& msg) {
    view.PrintFooter(msg);
    GetKeyPress();
}
