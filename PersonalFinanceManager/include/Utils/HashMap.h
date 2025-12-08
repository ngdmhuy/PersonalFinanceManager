//
//  HashMap.h
//  PersonalFinanceManager
//
//  Created by Nguyen Dinh Minh Huy on 30/11/25.
//

#ifndef HashMap_h
#define HashMap_h

#include "HashStrategies.h"
#include "ArrayList.h"

#include <algorithm>

/**
 * @struct HashNode
 * @brief Internal node structure for the HashMap chaining collision resolution.
 */
template <typename K, typename V>
struct HashNode {
    K key;
    V value;
    HashNode* next;
    
    HashNode(K k, V v) : key(k), value(v), next(nullptr) {}
};

/**
 * @class HashMap
 * @brief Key-Value store implementing a Hash Table with Chaining.
 * * Uses 'HashStrategies.h' to handle hashing of different types (int, string, char*).
 */
template <typename K, typename V>
class HashMap {
private:
    HashNode<K, V>** buckets; // Array of pointers to nodes
    size_t size;
    size_t capacity;
    double maxLoadFactor;
    size_t threshold;
    
    static const size_t DEFAULT_CAPACITY = 16;
    static constexpr double DEFAULT_LOAD_FACTOR = 0.75;
    
    /**
     * @brief Calculates the bucket index for a specific key.
     */
    unsigned long CalculateBucket(const K& key, size_t currentCapacity) const {
        // [Refactor] Updated to PascalCase call
        unsigned long rawHash = Hasher<K>::GetHash(key);
        return rawHash % currentCapacity;
    }

    /**
     * @brief Resizes internal storage when load factor is exceeded.
     */
    void Resize(size_t newCapacity) {
        HashNode<K, V>** newBuckets = new HashNode<K, V>*[newCapacity];
        for (size_t i = 0; i < newCapacity; ++i)
            newBuckets[i] = nullptr;

        for (size_t i = 0; i < capacity; ++i) {
            HashNode<K, V>* entry = buckets[i];
            while (entry != nullptr) {
                HashNode<K, V>* nextNode = entry->next;
                
                unsigned long newIndex = CalculateBucket(entry->key, newCapacity);
                
                entry->next = newBuckets[newIndex];
                newBuckets[newIndex] = entry;
                
                entry = nextNode;
            }
        }

        delete[] buckets;
        buckets = newBuckets;
        capacity = newCapacity;
        threshold = static_cast<size_t>(capacity * maxLoadFactor);
    }

public:
    // ==========================================
    // 1. CONSTRUCTORS & DESTRUCTOR
    // ==========================================
    
    HashMap(size_t initCap = DEFAULT_CAPACITY, double loadFactor = DEFAULT_LOAD_FACTOR)
        : size(0), maxLoadFactor(loadFactor) {
        
        this->capacity = std::max((size_t)1, initCap);
        this->threshold = static_cast<size_t>(this->capacity * maxLoadFactor);
        
        buckets = new HashNode<K, V>*[this->capacity];
        for (size_t i = 0; i < capacity; ++i)
            buckets[i] = nullptr;
    }

    ~HashMap() {
        Clear();
        delete[] buckets;
    }

    // ==========================================
    // 2. CORE OPERATIONS (CRUD)
    // ==========================================

    void Put(K key, V value) {
        if (size >= threshold) Resize(capacity * 2);

        unsigned long bucketIndex = CalculateBucket(key, capacity);
        HashNode<K, V>* entry = buckets[bucketIndex];
        
        while (entry != nullptr) {
            if (KeyComparer<K>::AreEqual(entry->key, key)) {
                entry->value = value;
                return;
            }
            entry = entry->next;
        }
        
        HashNode<K, V>* newNode = new HashNode<K, V>(key, value);
        newNode->next = buckets[bucketIndex];
        buckets[bucketIndex] = newNode;
        
        ++size;
    }

    void Remove(const K& key) {
        unsigned long bucketIndex = CalculateBucket(key, capacity);
        HashNode<K, V>* prev = nullptr;
        HashNode<K, V>* entry = buckets[bucketIndex];

        while (entry != nullptr) {
            if (KeyComparer<K>::AreEqual(entry->key, key)) {
                if (prev == nullptr) buckets[bucketIndex] = entry->next;
                else prev->next = entry->next;
                
                delete entry;
                size--;
                return;
            }
            prev = entry;
            entry = entry->next;
        }
    }

    void Clear() {
        for (size_t i = 0; i < capacity; ++i) {
            HashNode<K, V>* entry = buckets[i];
            while (entry != nullptr) {
                HashNode<K, V>* prev = entry;
                entry = entry->next;
                delete prev;
            }
            buckets[i] = nullptr;
        }
        size = 0;
    }

    // ==========================================
    // 3. ACCESSORS
    // ==========================================

    V* Get(K key) const {
        unsigned long bucketIndex = CalculateBucket(key, capacity);
        HashNode<K, V>* entry = buckets[bucketIndex];
        
        while (entry != nullptr) {
            if (KeyComparer<K>::AreEqual(entry->key, key)) {
                return &(entry->value);
            }
            entry = entry->next;
        }
        return nullptr;
    }

    V& operator[](const K& key) {
        unsigned long bucketIndex = CalculateBucket(key, capacity);
        HashNode<K, V>* entry = buckets[bucketIndex];

        while (entry != nullptr) {
            if (KeyComparer<K>::AreEqual(entry->key, key)) {
                return entry->value;
            }
            entry = entry->next;
        }

        if (size >= threshold) {
            Resize(capacity * 2);
            bucketIndex = CalculateBucket(key, capacity);
        }

        V defaultValue = V();
        HashNode<K, V>* newNode = new HashNode<K, V>(key, defaultValue);
        newNode->next = buckets[bucketIndex];
        buckets[bucketIndex] = newNode;
        size++;
        
        return newNode->value;
    }

    // ==========================================
    // 4. STATE & UTILITIES
    // ==========================================

    bool ContainsKey(K key) const { return Get(key) != nullptr; }

    size_t Count() const { return size; }

    bool IsEmpty() const { return size == 0; }
    
    ArrayList<K> Keys() {
        ArrayList<K> keys;
        for (size_t i = 0; i < capacity; ++i) {
            HashNode<K, V>* entry = buckets[i];
            while (entry != nullptr) {
                keys.Add(entry->key);
                entry = entry->next;
            }
        }
        return keys;
    }
    
    ArrayList<V> Values() {
        ArrayList<V> values;
        for (size_t i = 0; i < capacity; ++i) {
            HashNode<K, V>* entry = buckets[i];
            while (entry != nullptr) {
                values.Add(entry->value);
                entry = entry->next;
            }
        }
        return values;
    }
};

#endif // !HashMap_h
