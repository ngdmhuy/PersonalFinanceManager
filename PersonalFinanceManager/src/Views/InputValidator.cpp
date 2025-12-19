#include "Views/InputValidator.h"
#include "Views/ConsoleView.h"
#include <iostream>
#include <cctype>

using namespace std;

double InputValidator::GetValidMoney(const string& prompt) {
    double amount;
    while (true) {
        cout << prompt;
        if (cin >> amount && ValidateMoney(amount)) {
            cin.ignore(); // Clear input buffer
            return amount;
        }
        // Invalid input
        cin.clear();
        cin.ignore(10000, '\n');
        ConsoleView view;
        view.ShowError("Invalid input! Please enter an amount greater than 0.");
    }
}

Date InputValidator::GetValidDate(const string& prompt) {
    string dateStr;
    while (true) {
        cout << prompt;
        if (!std::getline(cin, dateStr)) return Date::GetTodayDate(); // EOF -> treat as today

        // Trim whitespace
        size_t start = dateStr.find_first_not_of(" \t\r\n");
        if (start == std::string::npos) {
            ConsoleView view;
            view.ShowError("Invalid date! Please enter date in format YYYY-MM-DD (or 'T' for today).");
            continue;
        }
        size_t end = dateStr.find_last_not_of(" \t\r\n");
        std::string token = dateStr.substr(start, end - start + 1);

        // Accept 'T', 't', or 'today' (case-insensitive) as today's date
        std::string lower = token;
        for (char &ch : lower) ch = (char)std::tolower(ch);
        if (lower == "t" || lower == "today") {
            return Date::GetTodayDate();
        }

        try {
            Date date = Date::FromString(token);
            if (date.IsValid()) {
                return date;
            }
        } catch (...) {
            // Invalid date format
        }

        ConsoleView view;
        view.ShowError("Invalid date! Please enter date in format YYYY-MM-DD (or 'T' for today).");
    }
}

string InputValidator::GetValidString(const string& prompt) {
    string input;
    while (true) {
        cout << prompt;
        getline(cin, input);
        
        if (ValidateString(input)) {
            return input;
        }
        
        ConsoleView view;
        view.ShowError("Invalid input! Text cannot be empty.");
    }
}

int InputValidator::GetValidIndex(const std::string& prompt, int min, int max, int x, int y) {
    std::string line;
    ConsoleView view;

    while (true) {
        // If a position is provided, move cursor there and clear the line so the prompt reuses the same spot
        if (x >= 0 && y >= 0) {
            view.MoveToXY(x, y);
            int fill = 80 - (x < 0 ? 0 : x);
            if (fill > 0) std::cout << std::string(fill, ' ');
            view.MoveToXY(x, y);
        }

        std::cout << prompt;
        std::cout << std::flush;
        if (!std::getline(std::cin, line)) return 0; // EOF -> cancel

        // Trim whitespace
        size_t start = line.find_first_not_of(" \t\r\n");
        if (start == std::string::npos) {
            // Empty line = cancel
            return 0;
        }
        size_t end = line.find_last_not_of(" \t\r\n");
        std::string token = line.substr(start, end - start + 1);

        // Allow explicit 0 to cancel
        if (token == "0") return 0;

        try {
            int value = std::stoi(token);
            if (value >= min && value <= max) return value;
            view.ShowError("Invalid selection. Please enter a number between " + std::to_string(min) + " and " + std::to_string(max) + ".");
        } catch (...) {
            view.ShowError("Invalid input. Please enter a number.");
        }
        // The loop will reposition the cursor and reprint the prompt if x/y were provided
    }
}

bool InputValidator::ValidateMoney(double amount) {
    return amount > 0;
}

bool InputValidator::ValidateString(const string& str) {
    // Remove leading/trailing whitespace check
    if (str.empty()) return false;
    
    // Check if string contains only whitespace
    for (char c : str) {
        if (!isspace(c)) return true;
    }
    return false;
}
