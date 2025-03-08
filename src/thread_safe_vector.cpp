#include "thread_safe_vector.h"
#include <cstddef>
#include <shared_mutex>
#include <stdexcept>

template <typename T>
ThreadSafeVector<T>::ThreadSafeVector(size_t initial_capacity)
    : capacity(initial_capacity), current_size(0) {

        this->data = new T[capacity];
}

template <typename T>
ThreadSafeVector<T>::~ThreadSafeVector() {
    delete[] this->data;
}

template <typename T>
void ThreadSafeVector<T>::push_back(const T& value) {
    std::unique_lock<std::shared_mutex> lock(rw_mutex);

    if(current_size == capacity) {
        resize(capacity * 2);
    }

    this->data[this->current_size++] = value;
}

template <typename T>
void ThreadSafeVector<T>::pop_back() {
    std::unique_lock<std::shared_mutex> lock(rw_mutex);

    if(this->current_size == 0) {
        throw std::runtime_error("vector is empty. cannot remove element");
    }

    this->data[this->current_size - 1] = T();
    this->current_size--;
}

template <typename T>
T& ThreadSafeVector<T>::operator[](size_t index) {
    std::shared_lock<std::shared_mutex> lock(rw_mutex);
    if(index >= current_size) {
        return -1;
    }
    return this->data[index];
}

template <typename T>
T& ThreadSafeVector<T>::at(size_t index) {
    std::shared_lock<std::shared_mutex> lock(rw_mutex);
    if(index >= current_size) {
        throw std::out_of_range("Index out of range");
    }
    return this->data[index];
}

template <typename T>
size_t ThreadSafeVector<T>::size() const {
    std::shared_lock<std::shared_mutex> lock(rw_mutex);
    return this->current_size;
}

template <typename T>
bool ThreadSafeVector<T>::empty() const {
    std::shared_lock<std::shared_mutex> lock(rw_mutex);
    return this->current_size == 0;
}

template <typename T>
void ThreadSafeVector<T>::clear() {
    std::unique_lock<std::shared_mutex> lock(rw_mutex);

    delete[] data;
    this->capacity = 10;
    this->data = new T[capacity];
    this->current_size = 0;
}
