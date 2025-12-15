#include <iostream>
#include "../../include/Controllers/AppController.h"
#include "../../include/Controllers/NavigationController.h"

int main() {
    try {
        // Initialize app controller (handles data persistence and business logic)
        AppController appController;

        // Initialize navigation controller (handles UI and user flow)
        NavigationController navController(&appController);

        // Load data from binary files
        navController.Init();

        // Start the main application loop
        navController.Run();

        // Save data and cleanup
        navController.Shutdown();

        return 0;
    }
    catch (const std::exception& e) {
        std::cerr << "Fatal error: " << e.what() << std::endl;
        return 1;
    }
}