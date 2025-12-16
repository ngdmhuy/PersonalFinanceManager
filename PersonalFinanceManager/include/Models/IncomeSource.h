//
//  IncomeSource.h
//  PersonalFinanceManager
//
//  Created by Nguyen Dinh Minh Huy on 5/12/25.
//

#ifndef IncomeSource_h
#define IncomeSource_h

#include "../Utils/BinaryFileHelper.h"

#include <iostream>
#include <fstream>
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
    
    // Serialization
    /**
     * @brief Serializes the object to a binary stream.
     * Order: ID -> Name
     */
    void ToBinary(std::ofstream& fout) const;

    /**
     * @brief Factory method to create a IncomeSource from a binary stream.
     * Reads in the same order as ToBinary.
     */
    static IncomeSource* FromBinary(std::ifstream& fin);
};

#endif // !IncomeSource_h
