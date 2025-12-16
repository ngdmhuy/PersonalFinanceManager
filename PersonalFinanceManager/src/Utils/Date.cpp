//
//  Date.cpp
//  PersonalFinanceManager
//
//  Created by Nguyen Dinh Minh Huy on 5/12/25.
//

#include "../../include/Utils/Date.h"

// ==========================================
// 1. CONSTRUCTORS
// ==========================================

Date::Date(): day(0), month(0), year(0) { }
Date::Date(int d, int m, int y) : day(d), month(m), year(y) { }

Date::~Date() = default;

// ==========================================
// 2. GETTERS
// ==========================================

int Date::GetDay() const { return day; }
int Date::GetMonth() const { return month; }
int Date::GetYear() const { return year; }

// ==========================================
// 3. OPERATORS
// ==========================================

bool Date::operator<(const Date& other) const {
    if (year != other.year) return year < other.year;
    if (month != other.month) return month < other.month;
    return day < other.day;
}

bool Date::operator>(const Date& other) const { return other < *this; }

bool Date::operator==(const Date& other) const {
    return day == other.day && month == other.month && year == other.year;
}

bool Date::operator<=(const Date& other) const { return !(*this > other); }
bool Date::operator>=(const Date& other) const { return !(*this < other); }
bool Date::operator!=(const Date& other) const { return !(*this == other); }

// ==========================================
// 4. UTILITIES & STATIC HELPERS
// ==========================================

Date Date::FromString(const std::string& dateStr) {
    if (dateStr.empty()) return Date();
    int d, m, y;
    char dash;
    // Expected format: YYYY-MM-DD
    std::stringstream ss(dateStr);
    ss >> y >> dash >> m >> dash >> d;
    return Date(d, m, y);
}

std::string Date::ToString() const {
    std::stringstream ss;
    ss << std::setw(4) << std::setfill('0') << year << "-"
       << std::setw(2) << std::setfill('0') << month << "-"
       << std::setw(2) << std::setfill('0') << day;
    
    return ss.str();
}

bool Date::IsLeapYear(int y) {
    return (y % 4 == 0 && y % 100 != 0) || (y % 400 == 0);
}

int Date::DaysInMonth(int m, int y) {
    if (m == 2) return IsLeapYear(y) ? 29 : 28;
    if (m == 4 || m == 6 || m == 9 || m == 11) return 30;
    return 31;
}

Date Date::GetEndOfMonth(int m, int y) {
    return Date(DaysInMonth(m, y), m, y);
}

bool Date::IsValid() const {
    if (year < 1900 || month < 1 || month > 12 || day < 1) return false;
    return day <= DaysInMonth(month, year);
}

Date Date::GetTodayDate() {
    time_t now = time(0);
    tm ltm;
    
#ifdef _WIN32
    localtime_s(&ltm, &now);
#else
    localtime_r(&now, &ltm);
#endif
    
    Date today;
    today.year = 1900 + ltm.tm_year;
    today.month = 1 + ltm.tm_mon;
    today.day = ltm.tm_mday;
    return today;
}

// Stream Operators
std::istream& operator>>(std::istream& is, Date& d) {
    std::string dateStr;
    is >> dateStr;
    d = Date::FromString(dateStr);
    return is;
}

std::ostream& operator<<(std::ostream& os, const Date& d) {
    os << d.ToString();
    return os;
}
