//
//  HashMap.h
//  PersonalFinanceManager
//
//  Created by Nguyen Dinh Minh Huy on 30/11/25.
//

#ifndef HashMap_h
#define HashMap_h

#include "HashStrategies.h"

#include <algorithm>

template <typename K, typename V>
struct HashNode {
    K key;
    V value;
    HashNode* next;
    
    HashNode(K k, V v) : key(k), value(v), next(nullptr) {}
};

template <typename K, typename V>
class HashMap {
private:
    HashNode<K, V>** buckets; // Array of pointers to nodes
    size_t size;
    size_t capacity;
    double maxLoadFactor
    size_t threshold;
    
    static const size_t DEFAULT_CAPACITY = 16;
    static const double DEFAULT_LOAD_FACTOR = 0.75;
    
    /**
     * Calculates the bucket index for a specific key.
     * Uses the Hasher strategy to support different key types.
     */
    unsigned long calculateBucket(const K& key, size_t currentCapacity) const {
        unsigned long rawHash = Hasher<K>::getHash(key);
        return rawHash % currentCapacity;
    }

    /**
     * Resizes the internal array when the load factor is exceeded.
     * Rehashes all existing nodes into the new buckets.
     */
    void resize(size_t newCapacity) {
        // 1. Allocate new, larger memory
        HashNode<K, V>** newBuckets = new HashNode<K, V>*[newCapacity];
        for (size_t i = 0; i < newCapacity; ++i)
            newBuckets[i] = nullptr;

        // 2. Rehash all existing nodes
        for (size_t i = 0; i < capacity; ++i) {
            HashNode<K, V>* entry = buckets[i];
            while (entry != nullptr) {
                HashNode<K, V>* nextNode = entry->next; // Save pointer to next
                
                // Recalculate index for new capacity
                unsigned long newIndex = calculateBucket(entry->key, newCapacity);
                
                // Insert at HEAD of the new bucket (O(1) operation)
                entry->next = newBuckets[newIndex];
                newBuckets[newIndex] = entry;
                
                entry = nextNode; // Move to next node in old chain
            }
        }

        // 3. Clean up old array shell (nodes were moved, not deleted)
        delete[] buckets;
        
        // 4. Update state
        buckets = newBuckets;
        capacity = newCapacity;
        threshold = static_cast<size_t>(capacity * maxLoadFactor);
    }
}
public:
    // --- Lifecycle Methods ---

    /**
     * Main Constructor.
     * Uses default arguments to handle empty init calls: HashMap map;
     */
    HashMap(size_t initCap = DEFAULT_CAPACITY, double loadFactor = DEFAULT_LOAD_FACTOR)
        : count(0), maxLoadFactor(loadFactor) {
        
        this->capacity = std::max((size_t)1, initCap);
        this->threshold = static_cast<size_t>(this->capacity * maxLoadFactor);
        
        buckets = new HashNode<K, V>*[this->capacity];
        for (size_t i = 0; i < capacity; ++i)
            buckets[i] = nullptr;
    }

    /**
     * Destructor.
     * Cleans up all nodes and the bucket array.
     */
    ~HashMap() {
        Clear();
        delete[] buckets;
    }

    // --- Core Operations (CRUD) ---

    /**
     * Inserts a key-value pair. Updates value if key exists.
     */
    void Put(K key, V value) {
        // 1. Check if resize is needed BEFORE adding
        if (count >= threshold) resize(capacity * 2);

        unsigned long bucketIndex = calculateBucket(key, capacity);
        HashNode<K, V>* entry = buckets[bucketIndex];
        
        // 2. Check for Update (Key already exists)
        while (entry != nullptr) {
            if (KeyComparer<K>::areEqual(entry->key, key)) {
                entry->value = value;
                return;
            }
            entry = entry->next;
        }
        
        // 3. Insert New Node at HEAD (Fastest method)
        // MUST use 'new' to allocate on heap, otherwise it deletes after scope ends
        HashNode<K, V>* newNode = new HashNode<K, V>(key, value);
        newNode->next = buckets[bucketIndex];
        buckets[bucketIndex] = newNode;
        
        ++count;
    }

    /**
     * Removes a key-value pair from the map.
     */
    void Remove(const K& key) {
        unsigned long bucketIndex = calculateBucket(key, capacity);
        HashNode<K, V>* prev = nullptr;
        HashNode<K, V>* entry = buckets[bucketIndex];

        while (entry != nullptr) {
            if (KeyComparer<K>::areEqual(entry->key, key)) {
                if (prev == nullptr) buckets[bucketIndex] = entry->next; // Removing Head
                else prev->next = entry->next; // Removing Middle/Tail
                
                delete entry;
                count--;
                return;
            }
            prev = entry;
            entry = entry->next;
        }
    }

    /**
     * Removes all items but keeps the map ready for reuse.
     */
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
        count = 0;
    }

    // --- Accessors ---

    /**
     * Returns a pointer to the value, or nullptr if not found.
     * Returning a pointer allows checking for existence without exception.
     */
    V* Get(K key) const {
        unsigned long bucketIndex = calculateBucket(key, capacity);
        HashNode<K, V>* entry = buckets[bucketIndex];
        
        while (entry != nullptr) {
            if (KeyComparer<K>::areEqual(entry->key, key)) {
                return &(entry->value);
            }
            entry = entry->next;
        }
        return nullptr;
    }

    /**
     * Subscript Operator.
     * Returns reference to value. Creates default if key missing.
     * Usage: map["Food"] = 100;
     */
    V& operator[](const K& key) {
        unsigned long bucketIndex = calculateBucket(key, capacity);
        HashNode<K, V>* entry = buckets[bucketIndex];

        // 1. Search existing
        while (entry != nullptr) {
            if (KeyComparer<K>::areEqual(entry->key, key)) {
                return entry->value;
            }
            entry = entry->next;
        }

        // 2. Not found? Create default value
        if (count >= threshold) {
            resize(capacity * 2);
            bucketIndex = calculateBucket(key, capacity); // Re-calc index after resize
        }

        V defaultValue = V(); // Calls default constructor of type V
        HashNode<K, V>* newNode = new HashNode<K, V>(key, defaultValue);
        newNode->next = buckets[bucketIndex];
        buckets[bucketIndex] = newNode;
        count++;
        
        return newNode->value;
    }

    // --- State Checks ---

    bool ContainsKey(K key) const { return Get(key) != nullptr; }

    size_t Size() const { return count; }

    bool IsEmpty() const { return count == 0; }
    
};

#endif /* HashMap_h */
