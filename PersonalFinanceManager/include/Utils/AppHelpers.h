//
//  AppHelpers.h
//  PersonalFinanceManager
//
//  Created by Nguyen Dinh Minh Huy on 21/12/25.
//

#ifndef AppHelpers_h
#define AppHelpers_h

#include <string>
#include <fstream>
#include "Models/Transaction.h"
#include "Utils/ArrayList.h"
#include "Utils/HashMap.h"
#include "Utils/BinaryFileHelper.h"

namespace AppHelpers {

bool IsStringEmptyOrWhitespace(const std::string& str);

// ==========================================
// 2. SORTING & SEARCH UTILS
// ==========================================
bool CompareTransactionsByDate(Transaction* const& a, Transaction* const& b);

    // Binary Search to find insertion point (Keep list sorted)
size_t GetSortedInsertIndex(ArrayList<Transaction*>* list, Date date);

// ==========================================
// 3. MEMORY MANAGEMENT UTILS
// ==========================================

// Specific cleanup for Index Maps (Value is ArrayList<Transaction*>*)
void ClearIndexMap(HashMap<std::string, ArrayList<Transaction*>*>* indexMap);
void AddToIndexMap(HashMap<std::string, ArrayList<Transaction*>*>* indexMap, const std::string& key, Transaction* t);
void RemoveFromIndexMap(HashMap<std::string, ArrayList<Transaction*>*>* indexMap, const std::string& key, Transaction* t);

// Generic cleanup for any ArrayList of Pointers
template <typename T>
void FreeList(ArrayList<T*>*& list) {
    if (list) {
        for (int i = 0; i < list->Count(); ++i) {
            delete list->Get(i);
        }
        delete list;
        list = nullptr;
    }
}

// ==========================================
// 4. FILE I/O UTILS (TEMPLATES)
// ==========================================

template <typename T>
void SaveTable(const std::string& filename, ArrayList<T*>* list) {
    std::ofstream fout(filename, std::ios::binary);
    if (fout.is_open()) {
        BinaryFileHelper::WriteList(fout, list);
        fout.close();
    }
}

template <typename T>
void LoadTable(const std::string& filename, ArrayList<T*>* list, HashMap<std::string, T*>* map) {
    std::ifstream fin(filename, std::ios::binary);
    if (fin.is_open()) {
        BinaryFileHelper::ReadList(fin, list);
        fin.close();
        
        // Re-populate the ID Map
        for (size_t i = 0; i < list->Count(); ++i) {
            T* obj = list->Get(i);
            map->Put(obj->GetId(), obj);
        }
    }
}

}

#endif /* AppHelpers_h */
