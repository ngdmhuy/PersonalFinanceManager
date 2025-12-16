//
//  Category.h
//  PersonalFinanceManager
//
//  Created by Nguyen Dinh Minh Huy on 5/12/25.
//

#ifndef Category_h
#define Category_h

#include "../Utils/BinaryFileHelper.h"

#include <iostream>
#include <fstream>
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
    
    // Serialization
    /**
     * @brief Serializes the object to a binary stream.
     * Order: ID -> Name
     */
    void ToBinary(std::ofstream& fout) const;

    /**
     * @brief Factory method to create a Category from a binary stream.
     * Reads in the same order as ToBinary.
     */
    static Category* FromBinary(std::ifstream& fin);
};

#endif // !Category_h
