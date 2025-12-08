//
//  ArrayList.h
//  PersonalFinanceManager
//
//  Created by Nguyen Dinh Minh Huy on 30/11/25.
//

#ifndef ArrayList_h
#define ArrayList_h

#include <iostream>
#include <stdexcept>
#include <algorithm>

/**
 * @class ArrayList
 * @brief A dynamic array implementation similar to std::vector or C# List<T>.
 * * Manages memory automatically, resizing as elements are added.
 * Provides bounds checking for safe access.
 *
 * @tparam T The type of elements stored in the list.
 */
template <typename T>
class ArrayList {
private:
    T* data;
    size_t size;
    size_t capacity;
    
    static const size_t DEFAULT_CAPACITY = 16;
    
    /**
     * @brief Resizes the internal array when capacity is reached.
     * Strategies: Double size to amortize cost (O(1) average).
     */
    void Resize(size_t newCapacity) {
        if (newCapacity < size) newCapacity = size;
        if (newCapacity == 0) newCapacity = 1;
        
        T* newData = new T[newCapacity];
        
        // Copy existing elements
        for (size_t i = 0; i < size; ++i) {
            newData[i] = data[i];
        }
        
        delete[] data;
        data = newData;
        this->capacity = newCapacity;
    }
    
public:
    /// Comparator function pointer type for sorting
    using Comparator = bool (*)(const T& a, const T& b);
    
    // ==========================================
    // 1. CONSTRUCTORS & DESTRUCTOR
    // ==========================================
    
    ArrayList() : size(0), capacity(DEFAULT_CAPACITY) {
        data = new T[capacity];
    }
    
    explicit ArrayList(size_t initCap) : size(0) {
        capacity = (initCap == 0) ? DEFAULT_CAPACITY : initCap;
        data = new T[capacity];
    }
    
    ArrayList(const T* other, size_t size) : size(size) {
        capacity = (size == 0) ? DEFAULT_CAPACITY : size * 2;
        data = new T[capacity];
        for (size_t i = 0; i < size; ++i) {
            data[i] = other[i];
        }
    }
    
    ArrayList(size_t count, const T& initialValue) : size(count), capacity(count * 1.5) {
        if (capacity == 0) capacity = 16;
        data = new T[capacity];
        for (size_t i = 0; i < size; ++i) {
            data[i] = initialValue;
        }
    }
    
    // Copy Constructor (Deep Copy)
    ArrayList(const ArrayList& other) : size(other.size), capacity(other.capacity) {
        data = new T[capacity];
        for (size_t i = 0; i < size; ++i) {
            data[i] = other.data[i];
        }
    }
    
    ~ArrayList() {
        delete[] data;
    }
    
    // Assignment Operator (Deep Copy)
    ArrayList& operator=(const ArrayList& other) {
        if (this != &other) {
            delete[] data;
            
            size = other.size;
            capacity = other.capacity;
            data = new T[capacity];
            for (size_t i = 0; i < size; ++i) {
                data[i] = other.data[i];
            }
        }
        return *this;
    }
    
    // ==========================================
    // 2. ACCESSORS & OPERATORS
    // ==========================================
    
    T& operator[](size_t index) {
        if (index >= size) throw std::out_of_range("Index out of bounds");
        return data[index];
    }
    
    const T& operator[](size_t index) const {
        if (index >= size) throw std::out_of_range("Index out of bounds");
        return data[index];
    }
    
    size_t Count() const { return size; }
    
    bool IsEmpty() const { return size == 0; }
    
    T Get(size_t index) const {
        if (index >= size) throw std::out_of_range("Index out of bounds");
        return data[index];
    }
    
    void Set(size_t index, T element) {
        if (index >= size) throw std::out_of_range("Index out of bounds");
        data[index] = element;
    }
    
    // ==========================================
    // 3. MUTATION (ADD/REMOVE)
    // ==========================================
    
    void Add(const T& element) {
        if (size >= capacity)
            Resize(capacity * 2);
        
        data[size++] = element;
    }
    
    void AddRange(const ArrayList<T>& other) {
        size_t newSize = size + other.size;
        if (newSize > capacity) {
            Resize(newSize * 1.5);
        }

        for (size_t i = 0; i < other.size; ++i) {
            data[size++] = other.data[i];
        }
    }
    
    void Fill(const T& value) {
        for (size_t i = 0; i < size; ++i) {
            data[i] = value;
        }
    }
    
    void Insert(size_t index, const T& element) {
        if (index > size) throw std::out_of_range("Index out of bounds");
        
        if (size >= capacity) {
            Resize(capacity * 2);
        }

        for (size_t i = size; i > index; --i) {
            data[i] = data[i - 1];
        }

        data[index] = element;
        ++size;
    }
    
    void RemoveAt(size_t index) {
        if (index >= size) return;
        
        for (size_t i = index; i < size - 1; ++i) {
            data[i] = data[i + 1];
        }
        
        size--;
    }
    
    void Remove(const T& element) {
        int index = IndexOf(element);
        if (index >= 0) RemoveAt(static_cast<size_t>(index));
    }
    
    void Clear() { size = 0; }
    
    // ==========================================
    // 4. SEARCH & SORT
    // ==========================================
    
    int IndexOf(const T& element) const {
        for (size_t i = 0; i < size; ++i)
            if (data[i] == element) return static_cast<int>(i);
        return -1;
    }
    
    bool Contains(const T& element) const {
        return IndexOf(element) != -1;
    }
    
    void Sort(Comparator comp) {
        if (size > 1) std::sort(data, data + size, comp);
    }
    
    // ==========================================
    // 5. ITERATORS (Range-based for loop)
    // ==========================================
    T* begin() { return data; }
    T* end() { return data + size; }
    const T* begin() const { return data; }
    const T* end() const { return data + size; }
};

#endif // !ArrayList_h
