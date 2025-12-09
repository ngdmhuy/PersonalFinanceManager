#ifndef CONSOLEVIEW_H
#define CONSOLEVIEW_H

#include <iostream>
#include <windows.h>
#include <string>
using namespace std;

class ConsoleView {
private:
    // Color constants for consistent UI theming
    static const int COLOR_SUCCESS = 10;      /// Light Green for success messages
    static const int COLOR_ERROR = 12;        /// Light Red for error messages
    static const int COLOR_WARNING = 14;      /// Light Yellow for warnings
    static const int COLOR_INFO = 11;         /// Light Cyan for info/status
    static const int COLOR_NORMAL = 7;        /// White for normal text
    static const int COLOR_HEADER = 9;        /// Light Blue for headers/titles

public:
    // ===== CONSOLE CONTROL FUNCTIONS =====
    
    /// @brief Moves console cursor to specified position (x, y)
    /// @param x Horizontal position (column) from left edge (0 = leftmost)
    /// @param y Vertical position (row) from top edge (0 = topmost)
    void MoveToXY(int x, int y);

    /// @brief Changes console text color and background
    /// @param color Color code (0-15): 0=Black, 7=White, 10=LightGreen, 12=LightRed
    void SetColor(int color);

    /// @brief Clears entire console screen and resets cursor to (0,0)
    void ClearScreen();

    /// @brief Resets console color to default white text on black background
    void ResetColor();

    // ===== DRAWING PRIMITIVES =====

    /// @brief Draws horizontal line at specified position
    /// @param x Starting column position of the line
    /// @param y Row position where line will be drawn
    /// @param length Number of characters in the line
    /// @param ch Character to repeat (default '-') 
    void PrintLine(int x, int y, int length, char ch = '-');

    /// @brief Draws rectangular box with borders
    /// @param x Left column position of box top-left corner
    /// @param y Top row position of box top-left corner
    /// @param width Width of box in characters (including borders)
    /// @param height Height of box in characters (including borders)
    void PrintBox(int x, int y, int width, int height);

    /// @brief Prints centered header with colored background at top of screen
    /// @param title Text content to display as header (max 40 chars recommended)
    void PrintHeader(string title);

    /// @brief Prints footer message at bottom of screen (row 24)
    /// @param message Instructional text or status (max 70 chars recommended)
    void PrintFooter(string message);

    /// @brief Prints shortcut footer with keyboard hints and status info
    /// @param shortcuts Keyboard shortcut instructions (e.g. "[1-5] Select | ESC Exit")
    /// @param status Current application status (e.g. "File saved ✓ | 25 transactions")
    void PrintShortcutFooter(string shortcuts, string status);

    // ===== MESSAGE DISPLAY FUNCTIONS =====

    /// @brief Displays success message with green color and checkmark icon
    /// @param message Success notification text (e.g. "Income added successfully!")
    void ShowSuccess(string message);

    /// @brief Displays error message with red color and error icon
    /// @param message Error description (e.g. "Invalid amount! Must be > 0")
    void ShowError(string message);

    /// @brief Displays warning message with yellow color and warning icon
    /// @param message Warning text (e.g. "Deleting wallet with transactions!")
    void ShowWarning(string message);

    /// @brief Displays info message with cyan color and info icon
    /// @param message Informational text (e.g. "Loading data from file...")
    void ShowInfo(string message);

    // ===== TABLE PRINTING FUNCTIONS =====

    /// @brief Prints table header row with column separators
    /// @param columns Array of column header texts (max 3 columns supported)
    /// @param colWidths Array of column widths in characters
    /// @param numCols Number of columns (1-3)
    void PrintTableHeader(string columns[], int colWidths[], int numCols);

    /// @brief Prints single data row in table format
    /// @param col1 First column data (left-aligned, padded to column width)
    /// @param col2 Second column data (right-aligned if numeric)
    /// @param col3 Third column data (currency format if applicable)
    void PrintTableRow(const string col1,const  string col2,const  string col3);

    /// @brief Prints horizontal separator line between table rows
    void PrintTableSeparator();

    /// @brief Prints currency value with thousand separators and VND suffix
    /// @param amount Numeric value to format (long integer)
    /// @return Formatted string (e.g. "50,000,000 VND")
    string FormatCurrency(long amount);
};

#endif
