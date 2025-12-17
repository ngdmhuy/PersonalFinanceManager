#include "Views/ConsoleView.h"
#include "Utils/PlatformUtils.h"
#include <sstream>

//CONSOLE CONTROL IMPLEMENTATIONS

void ConsoleView::MoveToXY(int x, int y) {
    MoveCursor(x, y);
}

void ConsoleView::SetColor(int color) {
    SetConsoleColor(color);
}

void ConsoleView::ClearScreen() {
    ::ClearScreen();
}

void ConsoleView::ResetColor() {
    SetConsoleColor(COLOR_NORMAL);
}

//DRAWING IMPLEMENTATIONS

void ConsoleView::PrintLine(int x, int y, int length, char ch) {
    MoveToXY(x, y);
    for (int i = 0; i < length; i++) {
        std::cout << ch;
    }
    std::cout << std::endl;
}

void ConsoleView::PrintBox(int x, int y, int width, int height) {
    // Top border: +-----+
    MoveToXY(x, y);
    std::cout << "+";
    for (int i = 0; i < width - 2; i++) std::cout << "-";
    std::cout << "+";

    // Middle: |     |
    for (int row = 1; row < height - 1; row++) {
        MoveToXY(x, y + row);
        std::cout << "|";
        for (int i = 0; i < width - 2; i++) std::cout << " ";
        std::cout << "|";
    }

    // Bottom border
    MoveToXY(x, y + height - 1);
    std::cout << "+";
    for (int i = 0; i < width - 2; i++) std::cout << "-";
    std::cout << "+";
}

void ConsoleView::PrintHeader(std::string title) {
    ClearScreen();

    // Top line
    SetColor(COLOR_HEADER);
    PrintLine(0, 0, 80, '=');

    // Title centered on line 1
    int startX = (80 - (int)(title.length())) / 2;
    if (startX < 0) startX = 0;
    MoveToXY(startX, 1);
    std::cout << title;

    // Separator line
    PrintLine(0, 3, 80, '-');
    ResetColor();
}

void ConsoleView::PrintFooter(std::string message) {
    // Separator
    MoveToXY(0, 23);
    PrintLine(0, 23, 80, '-');

    // Footer text
    MoveToXY(1, 24);
    SetColor(COLOR_INFO);
    std::cout << message;
    ResetColor();

    // Move cursor out of footer
    MoveToXY(0, 25);
}

void ConsoleView::PrintShortcutFooter(std::string shortcuts, std::string status) {
    // Separator
    MoveToXY(0, 23);
    PrintLine(0, 23, 80, '-');

    // Left part: shortcuts
    MoveToXY(1, 24);
    SetColor(COLOR_WARNING);
    std::cout << shortcuts;

    // Right part: status (right-aligned)
    int statusX = 80 - (int)(status.length()) - 1;
    if (statusX < (int)(shortcuts.length()) + 2) {
        statusX = (int)(shortcuts.length()) + 2;
    }
    MoveToXY(statusX, 24);
    SetColor(COLOR_INFO);
    std::cout << status;

    ResetColor();
    MoveToXY(0, 25);
}

//MESSAGE IMPLEMENTATIONS

void ConsoleView::ShowSuccess(std::string message) {
    SetColor(COLOR_SUCCESS);
    std::cout << "[OK] " << message << std::endl;
    ResetColor();
}

void ConsoleView::ShowError(std::string message) {
    SetColor(COLOR_ERROR);
    std::cout << "[ERR] " << message << std::endl;
    ResetColor();
}

void ConsoleView::ShowWarning(std::string message) {
    SetColor(COLOR_WARNING);
    std::cout << "[WARN] " << message << std::endl;
    ResetColor();
}

void ConsoleView::ShowInfo(std::string message) {
    SetColor(COLOR_INFO);
    std::cout << "[INFO] " << message << std::endl;
    ResetColor();
}

//TABLE IMPLEMENTATIONS

void ConsoleView::PrintTableHeader(std::string columns[], int colWidths[], int numCols) {
    std::cout << "+";
    for (int i = 0; i < numCols; i++) {
        for (int j = 0; j < colWidths[i]; j++) std::cout << "-";
        std::cout << "+";
    }
    std::cout << std::endl;

    std::cout << "|";
    for (int i = 0; i < numCols; i++) {
        std::cout << " " << std::left << std::setw(colWidths[i] - 1) << columns[i] << "|";
    }
    std::cout << std::endl;

    PrintTableSeparator(colWidths,numCols);
}

void ConsoleView::PrintTableRow(const std::string data[], const int colWidths[], int numCols) {
    std::cout << "|";
    for (int i = 0; i < numCols; i++) {
        // Left-align all columns and pad to the specified width
        std::cout << std::left << std::setw(colWidths[i]) << data[i] << "|";
    }
    std::cout << std::endl;
}

void ConsoleView::PrintTableSeparator(const int colWidths[], int numCols) {
    std::cout << "+";
    for (int i = 0; i < numCols; i++) {
        for (int j = 0; j < colWidths[i]; j++) std::cout << "-";
        std::cout << "+";
    }
    std::cout << std::endl;
}

std::string ConsoleView::FormatCurrency(long amount) {
    std::stringstream ss;
    ss << std::fixed << std::setprecision(0);
    std::string numStr = std::to_string(amount);
    std::string result;
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
