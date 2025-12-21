//
//  AppHelpers.cpp
//  PersonalFinanceManager
//
//  Created by Nguyen Dinh Minh Huy on 21/12/25.
//

#include "Models/Transaction.h"
#include "Utils/ArrayList.h"
#include "Utils/HashMap.h"
#include "Utils/BinaryFileHelper.h"
#include <fstream>
#include <string>

#include "Utils/AppHelpers.h"

namespace AppHelpers {

// --- VALIDATION ---
bool IsStringEmptyOrWhitespace(const std::string& str) {
    return str.find_first_not_of(' ') == std::string::npos;
}

// --- SORTING ---
bool CompareTransactionsByDate(Transaction* const& a, Transaction* const& b) {
    return a->GetDate() < b->GetDate();
}

size_t GetSortedInsertIndex(ArrayList<Transaction*>* list, Date date) {
    if (!list || list->Count() == 0) return 0;
    
    // Optimization: Check boundaries first
    if (date >= list->Get(list->Count() - 1)->GetDate()) return list->Count();
    if (date < list->Get(0)->GetDate()) return 0;

    size_t low = 0;
    size_t high = list->Count() - 1;
    
    while (low <= high) {
        size_t mid = low + (high - low) / 2;
        
        if (list->Get(mid)->GetDate() > date) {
            if (mid == 0) return 0; // Prevent underflow
            high = mid - 1;
        } else {
            low = mid + 1;
        }
    }
    return low;
}

// --- MEMORY ---
void ClearIndexMap(HashMap<std::string, ArrayList<Transaction*>*>* indexMap) {
    if (!indexMap) return;
    
    ArrayList<std::string> keys = indexMap->Keys();
    for (size_t i = 0; i < keys.Count(); ++i) {
        // Delete the bucket (ArrayList), but NOT the transactions inside
        delete *indexMap->Get(keys.Get(i));
    }
    delete indexMap;
}

void AddToIndexMap(HashMap<std::string, ArrayList<Transaction*>*>* indexMap, const std::string& key, Transaction* t) {
    if (IsStringEmptyOrWhitespace(key)) return;
    
    if (!indexMap->ContainsKey(key)) {
        indexMap->Put(key, new ArrayList<Transaction*>());
    }
    
    ArrayList<Transaction*>* list = *indexMap->Get(key);
    size_t pos = GetSortedInsertIndex(list, t->GetDate());
    list->Insert(pos, t);
}
void RemoveFromIndexMap(HashMap<std::string, ArrayList<Transaction*>*>* indexMap, const std::string& key, Transaction* t) {
    if (IsStringEmptyOrWhitespace(key)) return;
    
    if (indexMap->ContainsKey(key))
        (*indexMap->Get(key))->Remove(t);
}

}
