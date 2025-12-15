#ifndef INPUTVALIDATOR_H
#define INPUTVALIDATOR_H

#include <string>
#include "../Utils/Date.h"

class InputValidator {
public:
    /// @brief Prompts user for money input and validates it's greater than 0
    /// @param prompt Message to display to user
    /// @return double amount if valid, keeps asking until valid input
    static double GetValidMoney(const std::string& prompt);

    /// @brief Prompts user for date input and validates it's not default (0/0/0)
    /// @param prompt Message to display to user
    /// @return Date object if valid, keeps asking until valid input
    static Date GetValidDate(const std::string& prompt);

    /// @brief Prompts user for string input and validates it's not empty
    /// @param prompt Message to display to user
    /// @return std::string if valid, keeps asking until valid input
    static std::string GetValidString(const std::string& prompt);

private:
    /// @brief Validates if money amount is greater than 0
    /// @param amount Amount to validate
    /// @return true if amount > 0, false otherwise
    static bool ValidateMoney(double amount);

    /// @brief Validates if date is not default (0/0/0)
    /// @param date Date to validate
    /// @return true if date is valid, false otherwise
    static bool ValidateDate(const Date& date);

    /// @brief Validates if string is not empty
    /// @param str String to validate
    /// @return true if string is not empty, false otherwise
    static bool ValidateString(const std::string& str);
};

#endif // INPUTVALIDATOR_H