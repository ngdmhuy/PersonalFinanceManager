#ifndef NAVIGATIONCONTROLLER_H
#define NAVIGATIONCONTROLLER_H

#include <string>

// Controllers
#include "Controllers/AppController.h"

// Views
#include "Views/DashBoard.h"
#include "Views/Menus.h"
#include "Views/ConsoleView.h"
#include "Views/InputValidator.h"

class NavigationController {
public:
    NavigationController(AppController* app);
    ~NavigationController();

    // Initialize controller (load data, setup)
    void Init();

    // Start main navigation loop (blocks until exit)
    void Run();

    // Save and cleanup
    void Shutdown();

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

    // Helper: show message and wait for key
    void PauseWithMessage(const std::string& msg);
};

#endif // NAVIGATIONCONTROLLER_H
