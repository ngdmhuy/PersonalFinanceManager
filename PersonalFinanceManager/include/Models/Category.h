//
//  Category.h
//  PersonalFinanceManager
//
//  Created by Nguyen Dinh Minh Huy on 5/12/25.
//

#ifndef Category_h
#define Category_h

#include <string>

class Category {
private:
    std::string id;
    std::string name;
    
public:
    // Constructors
    Category();
    Category(std::string id, std::string name);
    
    // Destructors
    ~Category();
    
    // Getters
    std::string getId() const;
    std::string getName() const;
    
    // Setters
    void setName(const std::string& n);
    
    // Display
    std::string ToString() const;
};

#endif // !Category_h
