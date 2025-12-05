//
//  IncomeSource.cpp
//  PersonalFinanceManager
//
//  Created by Nguyen Dinh Minh Huy on 5/12/25.
//

#include "../../include/Models/IncomeSource.h"

// Constructors
IncomeSource::IncomeSource() { }
IncomeSource::IncomeSource(std::string id, std::string name) : id(id), name(name) { }

// Destructors
IncomeSource::~IncomeSource() = default;

// Getters
std::string IncomeSource::getId() const { return id; }
std::string IncomeSource::getName() const { return name; }

// Setters
void IncomeSource::setName(const std::string& n) { name = n; }

// Display
std::string IncomeSource::ToString() const {
    return name;
}
