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
    string data1[] = {"A","250000","12"};
    string data2[] = {"B","2120000000","3"};
    view.PrintTableHeader(headers, widths, numCols);
    view.PrintTableRow(data1, widths, numCols);
    view.PrintTableRow(data2, widths, numCols);
    view.PrintTableSeparator(widths,numCols);

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