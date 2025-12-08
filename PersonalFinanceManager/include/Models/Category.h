//
//  Category.h
//  PersonalFinanceManager
//
//  Created by Nguyen Dinh Minh Huy on 5/12/25.
//

#ifndef Category_h
#define Category_h

#include <string>

/**
 * @class Category
 * @brief Represents a classification for Expenses (e.g., Food, Rent).
 */
class Category {
private:
    std::string id;
    std::string name;
    
public:
    Category();
    Category(std::string id, std::string name);
    ~Category();
    
    // Accessors
    std::string GetId() const;
    std::string GetName() const;
    
    void SetName(const std::string& n);
    
    // Display
    std::string ToString() const;

    // TODO [M1]: Add toBinary(ofstream&) and fromBinary(ifstream&) here
};

#endif // !Category_h
