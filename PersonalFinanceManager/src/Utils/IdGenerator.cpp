//
//  IdGenerator.cpp
//  PersonalFinanceManager
//
//  Created by Nguyen Dinh Minh Huy on 5/12/25.
//

#include "../../include/Utils/IdGenerator.h"

char IdGenerator::GetRandomChar() {
    static const std::string charset = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_int_distribution<> dis(0, static_cast<int>(charset.size() - 1));
    
    return charset[dis(gen)];
}

std::string IdGenerator::GenerateId(const std::string& prefix = "") {
    std::stringstream ss;
    
    if (!prefix.empty()) ss << prefix << "-";
    
    for (int block = 0; block < BLOCK_NUMBER; ++block) {
        for (int i = 0; i < CHARS_PER_BLOCK; ++i)
            ss << GetRandomChar();
        if (block < BLOCK_NUMBER - 1) ss << "-";
    }
    
    return ss.str();
}
