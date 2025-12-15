#include "../../include/Views/ConsoleView.h"
#include <iomanip>
#include <sstream>

using namespace std;

//CONSOLE CONTROL IMPLEMENTATIONS

void ConsoleView::MoveToXY(int x, int y) {
    COORD coord = {(SHORT)x, (SHORT)y};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void ConsoleView::SetColor(int color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (WORD)color);
}

void ConsoleView::ClearScreen() {
    system("cls");
}

void ConsoleView::ResetColor() {
    SetColor(COLOR_NORMAL);
}

//DRAWING IMPLEMENTATIONS

void ConsoleView::PrintLine(int x, int y, int length, char ch) {
    MoveToXY(x, y);
    for (int i = 0; i < length; i++) {
        cout << ch;
    }
    cout << endl;
}

void ConsoleView::PrintBox(int x, int y, int width, int height) {
    // Top border: +-----+
    MoveToXY(x, y);
    cout << "+";
    for (int i = 0; i < width - 2; i++) cout << "-";
    cout << "+";

    // Middle: |     |
    for (int row = 1; row < height - 1; row++) {
        MoveToXY(x, y + row);
        cout << "|";
        for (int i = 0; i < width - 2; i++) cout << " ";
        cout << "|";
    }

    // Bottom border
    MoveToXY(x, y + height - 1);
    cout << "+";
    for (int i = 0; i < width - 2; i++) cout << "-";
    cout << "+";
}

void ConsoleView::PrintHeader(string title) {
    ClearScreen();

    // Top line
    SetColor(COLOR_HEADER);
    PrintLine(0, 0, 80, '=');

    // Title centered on line 1
    int startX = (80 - (int)(title.length())) / 2;
    if (startX < 0) startX = 0;
    MoveToXY(startX, 1);
    cout << title;

    // Separator line
    PrintLine(0, 3, 80, '-');
    ResetColor();
}

void ConsoleView::PrintFooter(string message) {
    // Separator
    MoveToXY(0, 23);
    PrintLine(0, 23, 80, '-');

    // Footer text
    MoveToXY(1, 24);
    SetColor(COLOR_INFO);
    cout << message;
    ResetColor();

    // Move cursor out of footer
    MoveToXY(0, 25);
}

void ConsoleView::PrintShortcutFooter(string shortcuts, string status) {
    // Separator
    MoveToXY(0, 23);
    PrintLine(0, 23, 80, '-');

    // Left part: shortcuts
    MoveToXY(1, 24);
    SetColor(COLOR_WARNING);
    cout << shortcuts;

    // Right part: status (right-aligned)
    int statusX = 80 - (int)(status.length()) - 1;
    if (statusX < (int)(shortcuts.length()) + 2) {
        statusX = (int)(shortcuts.length()) + 2;
    }
    MoveToXY(statusX, 24);
    SetColor(COLOR_INFO);
    cout << status;

    ResetColor();
    MoveToXY(0, 25);
}

//MESSAGE IMPLEMENTATIONS

void ConsoleView::ShowSuccess(string message) {
    SetColor(COLOR_SUCCESS);
    cout << "[OK] " << message << endl;
    ResetColor();
}

void ConsoleView::ShowError(string message) {
    SetColor(COLOR_ERROR);
    cout << "[ERR] " << message << endl;
    ResetColor();
}

void ConsoleView::ShowWarning(string message) {
    SetColor(COLOR_WARNING);
    cout << "[WARN] " << message << endl;
    ResetColor();
}

void ConsoleView::ShowInfo(string message) {
    SetColor(COLOR_INFO);
    cout << "[INFO] " << message << endl;
    ResetColor();
}

//TABLE IMPLEMENTATIONS

void ConsoleView::PrintTableHeader(string columns[], int colWidths[], int numCols) {
    cout << "+";
    for (int i = 0; i < numCols; i++) {
        for (int j = 0; j < colWidths[i]; j++) cout << "-";
        cout << "+";
    }
    cout << endl;

    cout << "|";
    for (int i = 0; i < numCols; i++) {
        cout << " " << left << setw(colWidths[i] - 1) << columns[i] << "|";
    }
    cout << endl;

    PrintTableSeparator();
}

void ConsoleView::PrintTableRow(const string col1, const string col2, const string col3) {
    // colWidths[] = {25, 25, 15}
    cout << "|" << left  << setw(25) << col1;  // Column 1: exactly 25
    cout << "|" << right << setw(25) << col2;  // Column 2: exactly 25
    cout << "|" << right << setw(15) << col3 << "|" << endl;  // Column 3: exactly 15
}

void ConsoleView::PrintTableSeparator() {
    // 25 + 25 + 15 + 4 ('+' at 4 places) = 79
    cout << "+-------------------------+-------------------------+---------------+" << endl;
}

string ConsoleView::FormatCurrency(long amount) {
    stringstream ss;
    ss << fixed << setprecision(0);
    string numStr = to_string(amount);
    string result;
    int count = 0;

    for (int i = (int)(numStr.length()) - 1; i >= 0; --i) {
        result = numStr[i] + result;
        count++;
        if (count % 3 == 0 && i > 0) {
            result = "," + result;
        }
    }
    return result + " VND";
}
