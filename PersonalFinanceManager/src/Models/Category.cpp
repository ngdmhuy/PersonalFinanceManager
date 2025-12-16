//
//  Category.cpp
//  PersonalFinanceManager
//
//  Created by Nguyen Dinh Minh Huy on 5/12/25.
//

#include "../../include/Models/Category.h"

Category::Category() { }

Category::Category(std::string id, std::string name)
    : id(id), name(name) {
}

Category::~Category() = default;

// Accessors
std::string Category::GetId() const { return id; }
std::string Category::GetName() const { return name; }

void Category::SetName(const std::string& n) { name = n; }

// Display
std::string Category::ToString() const {
    return name;
}

// Serialization
void Category::ToBinary(std::ofstream& fout) const {
    BinaryFileHelper::WriteString(fout, id);
    BinaryFileHelper::WriteString(fout, name);
}

Category* Category::FromBinary(std::ifstream& fin) {
    std::string id = BinaryFileHelper::ReadString(fin);
    std::string name = BinaryFileHelper::ReadString(fin);
    
    return new Category(id, name);
}
