//
//  IncomeSource.cpp
//  PersonalFinanceManager
//
//  Created by Nguyen Dinh Minh Huy on 5/12/25.
//

#include "../../include/Models/IncomeSource.h"

IncomeSource::IncomeSource() { }

IncomeSource::IncomeSource(std::string id, std::string name)
    : id(id), name(name) {
}

IncomeSource::~IncomeSource() = default;

// Accessors
std::string IncomeSource::GetId() const { return id; }
std::string IncomeSource::GetName() const { return name; }

void IncomeSource::SetName(const std::string& n) { name = n; }

// Display
std::string IncomeSource::ToString() const {
    return name;
}
