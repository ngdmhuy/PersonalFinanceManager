#include "Views/ConsoleView.h"
#include "Utils/PlatformUtils.h"
#include <sstream>

using namespace std;

//CONSOLE CONTROL IMPLEMENTATIONS

void ConsoleView::MoveToXY(int x, int y) {
    // Update internal cursor tracking when moving cursor
    cursorY = y;
    MoveCursor(x, y);
}


void ConsoleView::SetColor(int color) {
    SetConsoleColor(color);
}

void ConsoleView::ClearScreen() {
    ::ClearScreen();
    // Reset internal cursor tracking to top-left
    cursorY = 0;
}

void ConsoleView::ResetColor() {
    SetConsoleColor(COLOR_NORMAL);
}

//DRAWING IMPLEMENTATIONS

void ConsoleView::PrintLine(int x, int y, int length, char ch) {
    MoveToXY(x, y);
    for (int i = 0; i < length; i++) {
        cout << ch;
    }
    cout << endl;
    // After printing a full line and newline, update cursor row to the next line
    cursorY = y + 1;
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
    // Place footer at the line directly below content; ensure a minimum
    // separator Y so small content still shows footer in expected place.
    int sepY = (cursorY < 23) ? 23 : cursorY;
    PrintLine(0, sepY, 80, '-');

    // Footer text
    MoveToXY(1, sepY + 1);
    SetColor(COLOR_INFO);
    cout << message;
    ResetColor();

    // Move cursor out of footer
    cursorY = sepY + 2;
    MoveToXY(0, cursorY);
}

void ConsoleView::PrintShortcutFooter(string shortcuts, string status) {
    int sepY = (cursorY < 23) ? 23 : cursorY;
    PrintLine(0, sepY, 80, '-');

    // Left part: shortcuts
    MoveToXY(1, sepY + 1);
    SetColor(COLOR_WARNING);
    cout << shortcuts;

    // Right part: status (right-aligned)
    int statusX = 80 - (int)(status.length()) - 1;
    if (statusX < (int)(shortcuts.length()) + 2) {
        statusX = (int)(shortcuts.length()) + 2;
    }
    MoveToXY(statusX, sepY + 1);
    SetColor(COLOR_INFO);
    cout << status;

    ResetColor();
    cursorY = sepY + 2;
    MoveToXY(0, cursorY);
}

//MESSAGE IMPLEMENTATIONS

void ConsoleView::ShowSuccess(string message) {
    SetColor(COLOR_SUCCESS);
    cout << "[OK] " << message << endl;
    ResetColor();
    // Update cursor tracking since we emitted a newline
    cursorY++;
}

void ConsoleView::ShowError(string message) {
    SetColor(COLOR_ERROR);
    cout << "[ERR] " << message << endl;
    ResetColor();
    cursorY++;
}

void ConsoleView::ShowWarning(string message) {
    SetColor(COLOR_WARNING);
    cout << "[WARN] " << message << endl;
    ResetColor();
    cursorY++;
}

void ConsoleView::ShowInfo(string message) {
    SetColor(COLOR_INFO);
    cout << "[INFO] " << message << endl;
    ResetColor();
    cursorY++;
}

//TABLE IMPLEMENTATIONS

void ConsoleView::PrintTableHeader(string columns[], int colWidths[], int numCols) {
    // Print header line at current cursor row
    int y = cursorY;
    MoveToXY(0, y);
    cout << "+";
    for (int i = 0; i < numCols; i++) {
        for (int j = 0; j < colWidths[i]; j++) cout << "-";
        cout << "+";
    }
    cout << endl;
    cursorY = y + 1; y++;

    MoveToXY(0, y);
    cout << "|";
    for (int i = 0; i < numCols; i++) {
        cout << " " << left << setw(colWidths[i] - 1) << columns[i] << "|";
    }
    cout << endl;
    cursorY = y + 1; y++;

    // Separator after header
    PrintTableSeparator(colWidths,numCols);
}

void ConsoleView::PrintTableRow(const string data[], const int colWidths[], int numCols) {
    // Print row at current cursor row
    int y = cursorY;
    MoveToXY(0, y);
    cout << "|";
    for (int i = 0; i < numCols; i++) {
        // Left-align all columns and pad to the specified width
        cout << left << setw(colWidths[i]) << data[i] << "|";
    }
    cout << endl;
    cursorY = y + 1;
}

void ConsoleView::PrintTableSeparator(const int colWidths[], int numCols) {
    int y = cursorY;
    MoveToXY(0, y);
    cout << "+";
    for (int i = 0; i < numCols; i++) {
        for (int j = 0; j < colWidths[i]; j++) cout << "-";
        cout << "+";
    }
    cout << endl;
    cursorY = y + 1;
}

void ConsoleView::PrintText(const string &text) {
    cout << text << endl;
    // Advance internal cursor tracking because we emitted a newline
    cursorY++;
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
