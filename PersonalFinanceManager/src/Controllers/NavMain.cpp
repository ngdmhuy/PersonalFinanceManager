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


NavigationController::NavigationController(AppController* app) : appController(app), running(false) {}

NavigationController::~NavigationController() {}

void NavigationController::Init() {
    if (appController) {
        appController->LoadData();
        view.ShowInfo("Data loaded.");
    }
}

void NavigationController::Run() {
    running = true;

    while (running) {
        // Show dashboard (uses appController internally)
        dashboard.Display();

        // Show main menu and get choice
        char choice = menus.DisplayMainMenu();

        if (choice == 27) { // ESC
            running = false;
            break;
        }

        HandleMainMenuChoice(choice);
    }

    Shutdown();
}

void NavigationController::Shutdown() {
    if (appController) {
        appController->SaveData();
        view.ShowInfo("Data saved. Exiting...");
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
