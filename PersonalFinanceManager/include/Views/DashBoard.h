#ifndef DASHBOARD_H
#define DASHBOARD_H

#include "../Views/ConsoleView.h"

class Dashboard {
private:
    ConsoleView view;

public:
    /// @brief Displays main dashboard with total balance and wallet list
    void Display();

    /// @brief Show main menu options 
    void ShowMainMenu();
};

#endif 