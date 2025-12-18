//
//  Date.h
//  PersonalFinanceManager
//
//  Created by Nguyen Dinh Minh Huy on 4/12/25.
//

#ifndef DATE_H
#define DATE_H

#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
#include <ctime>

/**
 * @class Date
 * @brief Represents a calendar date (Day/Month/Year).
 * * Provides operators for comparison (<, >, ==) and helpers
 * for validation and formatting.
 */
class Date {
private:
    int day;
    int month;
    int year;
    
public:
    // ==========================================
    // 1. CONSTRUCTORS
    // ==========================================
    Date();
    Date(int d, int m, int y);
    ~Date();
    
    // ==========================================
    // 2. GETTERS
    // ==========================================
    int GetDay() const;
    int GetMonth() const;
    int GetYear() const;
    
    // ==========================================
    // 3. OPERATORS
    // ==========================================
    bool operator<(const Date& other) const;
    bool operator>(const Date& other) const;
    bool operator==(const Date& other) const;
    
    bool operator<=(const Date& other) const;
    bool operator>=(const Date& other) const;
    bool operator!=(const Date& other) const;
    
    // ==========================================
    // 4. UTILITIES
    // ==========================================
    
    /**
     * @brief Parses a string in "YYYY/MM/DD" format.
     */
    static Date FromString(const std::string& dateStr);
    
    /**
     * @brief Returns string in "YYYY/MM/DD" format.
     */
    std::string ToString() const;
    
    static bool IsLeapYear(int y);
    static int DaysInMonth(int m, int y);
    static Date GetEndOfMonth(int m, int y);
    
    /**
     * @brief Returns current system date.
     */
    static Date GetTodayDate();
    
    bool IsValid() const;
    
    friend std::istream& operator>>(std::istream& is, Date& d);
    friend std::ostream& operator<<(std::ostream& os, const Date& d);
};

#endif // DATE_H
