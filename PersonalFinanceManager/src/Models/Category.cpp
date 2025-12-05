//
//  Category.cpp
//  PersonalFinanceManager
//
//  Created by Nguyen Dinh Minh Huy on 5/12/25.
//

#include "../../include/Models/Category.h"

// Constructors
Category::Category() { }
Category::Category(std::string id, std::string name) : id(id), name(name) { }

// Destructors
Category::~Category() = default;

// Getters
std::string Category::getId() const { return id; }
std::string Category::getName() const { return name; }

// Setters
void Category::setName(const std::string& n) { name = n; }

// Display
std::string Category::ToString() const {
    return name;
}
