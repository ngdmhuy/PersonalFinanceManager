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
#include <limits>


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
            PauseWithMessage("Press any key to continue...");
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
                case '7':
                case '8':
                case '9':
                    HandleMainMenuChoice(choice);
                    break;
                default:
                    view.ShowError("Invalid selection. Try again.");
                    PauseWithMessage("Press any key to continue...");
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
            ShowSearchFlow();
            break;
        case '7':
            HandleClearData();
            break;
        case '8':
            HandleSaveData();
            break;
        case '9':
            running = false;
            break;
        default:
            view.ShowError("Invalid selection. Try again.");
            break;
    }
}

void NavigationController::HandleClearData() {
    view.ClearScreen();
    view.PrintHeader("DANGER ZONE: CLEAR DATA");
    view.ShowError("WARNING: This will delete ALL wallets, transactions, and settings.");
    view.ShowError("This action cannot be undone.");
    
    view.MoveToXY(5, 8);
    std::cout << "Are you sure you want to proceed? Type 'DELETE' to confirm: ";
    
    std::string confirm;
    std::cin >> confirm;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    
    if (confirm == "DELETE") {
        appController->ClearDatabase();
        PauseWithMessage("System reset complete. Press any key...");
    } else {
        view.ShowInfo("Operation cancelled.");
        PauseWithMessage("Press any key to return...");
    }
}

void NavigationController::HandleSaveData() {
    appController->SaveData();
    PauseWithMessage("Press any key to continue...");
}

void NavigationController::PauseWithMessage(const std::string& msg) {
    view.PrintFooter(msg);
    GetKeyPress();
}
