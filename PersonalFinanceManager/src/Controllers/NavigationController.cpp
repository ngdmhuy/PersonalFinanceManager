#include "Controllers/NavigationController.h"
#include <conio.h>
#include <iostream>

NavigationController::NavigationController(AppController* app) : appController(app), running(false) {}

NavigationController::~NavigationController() {}

void NavigationController::Init() {
    if (appController) {
        appController->loadData();
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
        appController->saveData();
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

void NavigationController::ShowIncomeFlow() {
    char c = menus.DisplayIncomeMenu();
    // Placeholder: actual handlers will be implemented later
    if (c == 27) return;
    view.ShowInfo("Income flow selected (placeholder)");
    PauseWithMessage("Press any key to return...");
}

void NavigationController::ShowExpenseFlow() {
    char c = menus.DisplayExpenseMenu();
    if (c == 27) return;
    view.ShowInfo("Expense flow selected (placeholder)");
    PauseWithMessage("Press any key to return...");
}

void NavigationController::ShowWalletFlow() {
    char c = menus.DisplayWalletMenu();
    if (c == 27) return;
    view.ShowInfo("Wallet flow selected (placeholder)");
    PauseWithMessage("Press any key to return...");
}

void NavigationController::ShowReportsFlow() {
    char c = menus.DisplayReportsMenu();
    if (c == 27) return;
    view.ShowInfo("Reports flow selected (placeholder)");
    PauseWithMessage("Press any key to return...");
}

void NavigationController::PauseWithMessage(const std::string& msg) {
    view.PrintFooter(msg);
    _getch();
}
