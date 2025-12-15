#include "../../include/Views/InputValidator.h"
#include "../../include/Views/ConsoleView.h"
#include <iostream>

using namespace std;

double InputValidator::GetValidMoney(const std::string& prompt) {
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

Date InputValidator::GetValidDate(const std::string& prompt) {
    string dateStr;
    while (true) {
        cout << prompt;
        getline(cin, dateStr);
        
        try {
            Date date = Date::FromString(dateStr);
            if (ValidateDate(date)) {
                return date;
            }
        } catch (...) {
            // Invalid date format
        }
        
        ConsoleView view;
        view.ShowError("Invalid date! Please enter date in format DD/MM/YYYY.");
    }
}

std::string InputValidator::GetValidString(const std::string& prompt) {
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

bool InputValidator::ValidateMoney(double amount) {
    return amount > 0;
}

bool InputValidator::ValidateDate(const Date& date) {
    // Check if date is not default (0/0/0)
    return !(date.getDay() == 0 && date.getMonth() == 0 && date.getYear() == 0);
}

bool InputValidator::ValidateString(const std::string& str) {
    // Remove leading/trailing whitespace check
    if (str.empty()) return false;
    
    // Check if string contains only whitespace
    for (char c : str) {
        if (!isspace(c)) return true;
    }
    return false;
}