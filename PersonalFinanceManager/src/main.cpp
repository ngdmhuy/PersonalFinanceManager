#include <iostream>
#include "Controllers/AppController.h"
#include "Controllers/NavigationController.h"
#include "Views/ConsoleView.h"
#include "Utils/PlatformUtils.h"

int main() {
    SetupConsole();
    
    try {
        // Initialize console view (centralized UI messaging)
        ConsoleView view;

        // Initialize app controller (auto-load existing data, handles data persistence and business logic)
        AppController appController(&view);

        // Initialize navigation controller (handles UI and user flow)
        NavigationController navController(&appController);

        // Start the main application loop
        navController.Run();

        // On exit, appController auto-saves data back to binary files
        return 0;
    }
    catch (const std::exception& e) {
        std::cerr << "Fatal error: " << e.what() << std::endl;
        return 1;
    }
}
