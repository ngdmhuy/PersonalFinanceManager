#include "../../include/Views/ConsoleView.h"
#include <iomanip>
#include <sstream>

// ===== CONSOLE CONTROL IMPLEMENTATIONS =====

void ConsoleView::MoveToXY(int x, int y) {
    COORD coord = {(short)x, (short)y};
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

// ===== DRAWING PRIMITIVES =====

void ConsoleView::PrintLine(int x, int y, int length, char ch) {
    MoveToXY(x,y);
    for (int i = 0; i < length; ++i) cout << ch;
    cout << endl;
}

void ConsoleView::PrintBox(int x, int y, int width, int height) {
    MoveToXY(x,y);
    cout << string(width,'=') << endl;
    for (int i = 1; i< height - 1; ++i) {
        MoveToXY(x,y+i);
        cout << "║" << string(width - 2, ' ') << '║' << endl;
    }
    MoveToXY(x, y + height - 1);
    cout << string(width, '=') << endl;
}


void ConsoleView::PrintHeader(string title) {
    ClearScreen();
    MoveToXY(0, 0);
    SetColor(COLOR_HEADER);
    PrintLine(0, 0, 80, '=');   
    MoveToXY((80 - title.length()) / 2, 1);
    cout << ' ' << title << ' ';
    PrintLine(0, 3, 80, '-');
    ResetColor();
}

void ConsoleView::PrintFooter(string message) {
    MoveToXY(0, 23);
    PrintLine(0, 23, 80, '-');
    MoveToXY(2, 24);
    SetColor(COLOR_INFO);
    cout << message;
    MoveToXY(0, 25);
    ResetColor();
}

void ConsoleView::PrintShortcutFooter(string shortcuts, string status) {
    MoveToXY(0, 23);
    PrintLine(0, 23, 80, '-');
    MoveToXY(2, 24);
    SetColor(COLOR_WARNING);
    cout << shortcuts;
    MoveToXY(50, 24);
    SetColor(COLOR_INFO);
    cout << status;
    ResetColor();
    MoveToXY(0, 25);
}

// ===== MESSAGE IMPLEMENTATIONS =====

void ConsoleView::ShowSuccess(string message) {
    SetColor(COLOR_SUCCESS);
    cout << "[✓] " << message << endl;
    ResetColor();
}

void ConsoleView::ShowError(string message) {
    SetColor(COLOR_ERROR);
    cout << "[✗] " << message << endl;
    ResetColor();
}

void ConsoleView::ShowWarning(string message) {
    SetColor(COLOR_WARNING);
    cout << "[!] " << message << endl;
    ResetColor();
}

void ConsoleView::ShowInfo(string message) {
    SetColor(COLOR_INFO);
    cout << message << endl;
    ResetColor();
}

// ===== TABLE IMPLEMENTATIONS =====

void ConsoleView::PrintTableHeader(string columns[], int colWidths[], int numCols) {
    PrintLine(0, 0, colWidths[0] + colWidths[1] + colWidths[2] + 5, '═');
    
    cout << "│";
    for (int i = 0; i < numCols; i++) {
        cout << " " << left << setw(colWidths[i]) << columns[i] << " │";
    }
    cout << endl;
    PrintTableSeparator();
}

void ConsoleView::PrintTableRow(string col1, string col2, string col3) {
    cout << "│ " << left << setw(18) << col1 
         << " │ " << right << setw(12) << col2 
         << " │ " << right << setw(14) << col3 << " │" << endl;
}

/// Prints separator between table rows
void ConsoleView::PrintTableSeparator() {
    cout << "├" << string(20, '─') << "┼" 
         << string(14, '─') << "┼" << string(16, '─') << "┤" << endl;
}

string ConsoleView::FormatCurrency(long amount) {
    stringstream ss;
    ss << fixed << setprecision(0);
    string numStr = to_string(amount);
    string result;
    int count = 0;
    
    for (int i = numStr.length() - 1; i >= 0; i--) {
        result = numStr[i] + result;
        count++;
        if (count % 3 == 0 && i > 0) {
            result = "," + result;
        }
    }
    return result + " VND";
}