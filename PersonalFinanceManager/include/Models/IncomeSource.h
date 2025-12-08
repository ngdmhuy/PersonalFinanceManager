//
//  IncomeSource.h
//  PersonalFinanceManager
//
//  Created by Nguyen Dinh Minh Huy on 5/12/25.
//

#ifndef IncomeSource_h
#define IncomeSource_h

#include <string>

/**
 * @class IncomeSource
 * @brief Represents a classification for Income (e.g., Salary, Freelance).
 */
class IncomeSource {
private:
    std::string id;
    std::string name;
    
public:
    IncomeSource();
    IncomeSource(std::string id, std::string name);
    ~IncomeSource();
    
    // Accessors
    std::string GetId() const;
    std::string GetName() const;
    
    void SetName(const std::string& n);
    
    // Display
    std::string ToString() const;

    // TODO [M1]: Add toBinary(ofstream&) and fromBinary(ifstream&) here
};

#endif // !IncomeSource_h
