#include "Views/DashBoard.h"
#include "Views/Menus.h"
#include "Utils/PlatformUtils.h"
#include "Models/Wallet.h"

void Dashboard::Display() {
    view.ClearScreen();
    view.PrintHeader("PERSONAL FINANCE MANAGER - DASHBOARD");

    // Display total balance
    double totalBalance = 0.0; // This would be calculated from wallets in a real app
    view.MoveToXY(5, 5);
    if (totalBalance < 0) view.SetColor(12); // Red color for negative balance
    else view.SetColor(10); // Green color for positive balance

    std::cout << "Total Balance: " << view.FormatCurrency(totalBalance) << std::endl;
    view.ResetColor();

    // Display wallet list (mock data for demonstration)
    std::string headers[] = {"Wallet Name", "Balance", "Transactions"};
    int widths[] = {25, 25, 15};
    int numCols = 3;
    std::string data1[] = {"A","250000","12"};
    std::string data2[] = {"B","2120000000","3"};
    view.PrintTableHeader(headers, widths, numCols);
    view.PrintTableRow(data1, widths, numCols);
    view.PrintTableRow(data2, widths, numCols);
    view.PrintTableSeparator(widths,numCols);

    view.PrintShortcutFooter("[M] Main Menu  [ESC] Exit", "Dashboard READY");

    // Wait for user input to navigate
    char choice = GetKeyPress();
    if (choice == 'm' || choice == 'M') {
        ShowMainMenu();
    }
}

void Dashboard::ShowMainMenu() {
    std::cout << "\n" << Menus::MAIN_MENU_TITLE << "\n";
    std::cout << Menus::MAIN_MENU_1 << "\n";
    std::cout << Menus::MAIN_MENU_2 << "\n";
    std::cout << Menus::MAIN_MENU_3 << "\n";
    std::cout << Menus::MAIN_MENU_4 << "\n";
    std::cout << Menus::MAIN_MENU_5 << "\n";
}
