#ifndef THREADSAFEVECTOR_H
#define THREADSAFEVECTOR_H

#include <cstddef>
#include <shared_mutex>

template <typename T>
class ThreadSafeVector {
private:
    T* data;
    size_t capacity;
    size_t current_size;
    mutable std::shared_mutex rw_mutex;

    void resize(size_t new_capacity) {
        T* new_data = new T[new_capacity];

        for(size_t i = 0; i < current_size; i++) {
            new_data[i] = data[i];
        }

        delete[] data;
        data = new_data;
        capacity = new_capacity;
    }
public:
    ThreadSafeVector(size_t initial_capacity = 10);
    ~ThreadSafeVector();

    void push_back(const T& value);
    void pop_back();
    T& operator[](size_t index);
    T& at(size_t index);
    size_t size() const;
    bool empty() const;
    void clear();
};

#endif // THREADSAFEVECTOR_H
