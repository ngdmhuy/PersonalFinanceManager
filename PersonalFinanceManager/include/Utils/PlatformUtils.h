#ifndef PlatformUtils_h
#define PlatformUtils_h

#include <iostream>
#include <string>

// ==========================================
// WINDOWS IMPLEMENTATION
// ==========================================
#ifdef _WIN32
#include <conio.h>
#include <windows.h>

inline void ClearScreen() {
    system("cls");
}

inline int GetKeyPress() {
    return _getch();
}

inline void MoveCursor(int x, int y) {
    COORD coord = {(SHORT)x, (SHORT)y};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

inline void SetConsoleColor(int color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (WORD)color);
}

inline void SetupConsole() {
    // Enable ANSI colors for Windows 10/11
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwMode = 0;
    GetConsoleMode(hOut, &dwMode);
    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(hOut, dwMode);
    SetConsoleOutputCP(CP_UTF8);
}

// ==========================================
// MACOS / LINUX IMPLEMENTATION
// ==========================================
#else
#include <cstdlib>
#include <unistd.h>
#include <termios.h>

inline void ClearScreen() {
    std::cout << "\033[2J\033[1;1H";
}

inline int GetKeyPress() {
    struct termios oldt, newt;
    int ch;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return ch;
}

inline void MoveCursor(int x, int y) {
    // ANSI: \033[<row>;<col>H (1-based)
    std::cout << "\033[" << (y + 1) << ";" << (x + 1) << "H";
}

inline void SetConsoleColor(int color) {
    // Map Windows legacy color codes to ANSI
    std::string code = "0";
    switch(color) {
        case 10: code = "92"; break; // Bright Green
        case 12: code = "91"; break; // Bright Red
        case 14: code = "93"; break; // Bright Yellow
        case 11: code = "96"; break; // Bright Cyan
        case 9:  code = "94"; break; // Bright Blue
        case 7:  code = "0";  break; // Reset
        default: code = "0";  break;
    }
    std::cout << "\033[" << code << "m";
}

inline void SetupConsole() {
    // No setup needed for Unix
}
#endif

#endif /* PlatformUtils_h */
