//
//  HashStrategies.h
//  PersonalFinanceManager
//
//  Created by Huy Nguyen on 1/12/25.
//

#ifndef HashStrategies_h
#define HashStrategies_h

#include <string>
#include <cstring>

// Default: Expects T to have a .hash() method
template <typename T>
struct Hasher {
    static unsigned long getHash(const T& key) { return key.hash(); }
};

// Specialization for Integers
template <>
struct Hasher<int> {
    static unsigned long getHash(int key) { return static_cast<unsigned long>(key); }
};

// Specialization for std::string
template <>
struct Hasher<std::string> {
    static unsigned long getHash(const std::string& str) {
        unsigned long hash = 5381;
        for (char c : str) {
            hash = ((hash << 5) + hash) + c;
        }
        return hash;
    }
};

// Specialization for C-Strings
template <>
struct Hasher<const char*> {
    static unsigned long getHash(const char* str) {
        unsigned long hash = 5381;
        int c;
        while ((c = *str++))
            hash = ((hash << 5) + hash) + c;
        return hash;
    }
};

// --- STRATEGY 2: Key Comparer ---
// Ensures comparisons work correctly for pointers vs values

template <typename T>
struct KeyComparer {
    static bool areEqual(const T& a, const T& b) { return a == b; }
};

// Specialization for C-Strings
template <>
struct KeyComparer<const char*> {
    static bool areEqual(const char* a, const char* b) {
        return std::strcmp(a, b) == 0;
    }
};

#endif /* HashStrategies_h */
