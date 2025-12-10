#include <conio.h> //for _getch()
#include "Views/ConsoleView.h"
#include "Views/DashBoard.h"
int main() {
    ConsoleView view;

    //TEST 1: MESSAGES & BASIC CONTROL
    view.ClearScreen();
    view.PrintHeader("CONSOLEVIEW TEST");

    view.ShowSuccess("MoveToXY, SetColor, ClearScreen OK");
    view.ShowError("Error message test");
    view.ShowWarning("Warning message test");
    view.ShowInfo("Info message test");

    view.PrintFooter("Test 1 Complete - Press any key to continue...");
    _getch(); 

    //TEST 2: BOX DRAWING
    view.ClearScreen();
    view.PrintHeader("BOX TEST");

    view.PrintBox(10, 5, 30, 8);
    view.MoveToXY(15, 8);
    view.SetColor(10);
    std::cout << "BOX TEST OK";
    view.ResetColor();

    view.PrintLine(0, 18, 80, '=');
    view.PrintFooter("Test 2 Complete - Press any key to continue...");
    _getch();

    //TEST 3: TABLE
    view.ClearScreen();
    view.PrintHeader("TABLE TEST");

    std::string headers[] = {"Wallet", "Balance", "Transactions"};
    int widths[] = {25, 25, 15};
    int numCols = 3;

    view.PrintTableHeader(headers, widths, numCols);
    view.PrintTableRow("Cash", view.FormatCurrency(5000000),  "12");
    view.PrintTableRow("Bank", view.FormatCurrency(45000000), "3");
    view.PrintTableSeparator();

    view.PrintShortcutFooter("[ESC] Exit", "ConsoleView READY");
    _getch();

    //TEST 4: DASHBOARD VIEW
    Dashboard dash;
    dash.Display();
    _getch();

    return 0;
}
