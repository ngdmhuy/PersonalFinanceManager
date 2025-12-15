#include "../../include/Views/DashBoard.h"
#include "../../include/Views/Menus.h"
#include <conio.h> // for _getch()

void Dashboard::Display() {
    view.ClearScreen();
    view.PrintHeader("PERSONAL FINANCE MANAGER - DASHBOARD");

    // Display total balance
    double totalBalance = 0.0; // This would be calculated from wallets in a real app
    view.MoveToXY(5, 5);
    if (totalBalance < 0) view.SetColor(12); // Red color for negative balance
    else view.SetColor(10); // Green color for positive balance

    std::cout << "Total Balance: " << view.FormatCurrency(totalBalance) << endl;
    view.ResetColor();

    // Display wallet list (mock data for demonstration)
    std::string headers[] = {"Wallet Name", "Balance", "Transactions"};
    int widths[] = {25, 25, 15};
    int numCols = 3;

    view.PrintTableHeader(headers, widths, numCols);
    view.PrintTableRow("Cash", "Waiting M1 Wallet data", "12");
    view.PrintTableRow("Bank", "Waiting M1 Wallet data", "3");
    view.PrintTableSeparator();

    view.PrintShortcutFooter("[M] Main Menu  [ESC] Exit", "Dashboard READY");

    // Wait for user input to navigate
    char choice = _getch();
    if (choice == 'm' || choice == 'M') {
        ShowMainMenu();
    }
}

void Dashboard::ShowMainMenu() {
    cout << "\n" << Menus::MAIN_MENU_TITLE << "\n";
    cout << Menus::MAIN_MENU_1 << "\n";
    cout << Menus::MAIN_MENU_2 << "\n";
    cout << Menus::MAIN_MENU_3 << "\n";
    cout << Menus::MAIN_MENU_4 << "\n";
    cout << Menus::MAIN_MENU_5 << "\n";
}