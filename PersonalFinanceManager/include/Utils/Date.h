//
//  Date.h
//  PersonalFinanceManager
//
//  Created by Nguyen Dinh Minh Huy on 4/12/25.
//

#ifndef Date_h
#define Date_h

#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
#include <ctime>

class Date {
private:
    int day;
    int month;
    int year;
    
public:
    // Constructors
    Date();
    Date(int d, int m, int y);
    
    // Destructors
    ~Date();
    
    // Getters
    int getDay() const;
    int getMonth() const;
    int getYear() const;
    
    // Operators
    bool operator<(const Date& other) const;
    bool operator>(const Date& other) const;
    bool operator==(const Date& other) const;
    
    bool operator<=(const Date& other) const;
    bool operator>=(const Date& other) const;
    bool operator!=(const Date& other) const;
    
    // Others
    
    static Date FromString(const std::string& dateStr);
    
    std::string ToString() const;
    static bool IsLeapYear(int y);
    
    static int DaysInMonth(int m, int y);
    
    static Date GetEndOfMonth(int m, int y);
    
    bool IsValid() const;
    
    static Date GetTodayDate();
    
    friend std::istream& operator>>(std::istream& is, Date& d);
    
    friend std::ostream& operator<<(std::ostream& os, const Date& d);
};


#endif /* Date_h */
