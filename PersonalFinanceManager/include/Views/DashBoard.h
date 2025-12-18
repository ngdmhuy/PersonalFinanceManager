#ifndef Dashboard_h
#define Dashboard_h

#include "Views/ConsoleView.h"
#include "Utils/ArrayList.h"

// FORWARD DECLARATION
class Wallet;
class AppController;

class Dashboard {
private:
    ConsoleView view;
    AppController* appController = nullptr; // Pointer to app controller for live data

public:
    /// @brief Displays main dashboard with total balance and wallet list
    char Display();

    /// @brief Bind Dashboard to the AppController so it can read live data
    void SetAppController(AppController* app) { this->appController = app; }
};

#endif // !Dashboard_h
