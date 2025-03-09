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
    ThreadSafeVector(size_t initial_capacity = 10)
        : capacity(initial_capacity), current_size(0) {

            this->data = new T[capacity];
    }

    ~ThreadSafeVector() {
        delete[] this->data;
    }

    void push_back(const T& value) {
        std::unique_lock<std::shared_mutex> lock(rw_mutex);

        if(current_size == capacity) {
            resize(capacity * 2);
        }

        this->data[this->current_size++] = value;
    }

    void pop_back() {
        std::unique_lock<std::shared_mutex> lock(rw_mutex);

        if(this->current_size == 0) {
            throw std::runtime_error("vector is empty. cannot remove element");
        }

        this->data[this->current_size - 1] = T();
        this->current_size--;
    }

    void erase(size_t index) {
        std::unique_lock<std::shared_mutex> lock(rw_mutex);

        if(index >= current_size) {
            throw std::out_of_range("Index out of range");
        }

        this->data[index] = this->data[--this->current_size];
    }

    T& operator[](size_t index) {
        std::shared_lock<std::shared_mutex> lock(rw_mutex);
        if(index >= current_size) {
            throw std::out_of_range("Index out of range");
        }
        return this->data[index];
    }

    T& at(size_t index) {
        std::shared_lock<std::shared_mutex> lock(rw_mutex);
        if(index >= current_size) {
            throw std::out_of_range("Index out of range");
        }
        return this->data[index];
    }

    size_t size() const {
        std::shared_lock<std::shared_mutex> lock(rw_mutex);
        return this->current_size;
    }

    bool empty() const {
        std::shared_lock<std::shared_mutex> lock(rw_mutex);
        return this->current_size == 0;
    }

    void clear() {
        std::unique_lock<std::shared_mutex> lock(rw_mutex);

        delete[] data;
        this->capacity = 10;
        this->data = new T[capacity];
        this->current_size = 0;
    }
};

#endif // THREADSAFEVECTOR_H
