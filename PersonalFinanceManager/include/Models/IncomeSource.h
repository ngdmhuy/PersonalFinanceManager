//
//  IncomeSource.h
//  PersonalFinanceManager
//
//  Created by Nguyen Dinh Minh Huy on 5/12/25.
//

#ifndef IncomeSource_h
#define IncomeSource_h

#include <string>

class IncomeSource {
private:
    std::string id;
    std::string name;
    
public:
    // Constructors
    IncomeSource();
    IncomeSource(std::string id, std::string name);
    
    // Destructors
    ~IncomeSource();
    
    // Getters
    std::string getId() const;
    std::string getName() const;
    
    // Setters
    void setName(const std::string& n);
    
    // Display
    std::string ToString() const;
};

#endif // !IncomeSource_h
