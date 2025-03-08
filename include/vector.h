#include <cstddef>
#include <shared_mutex>

template <typename T>
class ThreadSafeVector {
private:
    T* data;
    size_t capacity;
    size_t current_size;
    mutable std::shared_mutex mutex;

    void resize(size_t new_capacity);
public:
    ThreadSafeVector(size_t initial_capacity = 10);
    ~ThreadSafeVector();

    void push_back(const T& value);
    T& operator[](size_t index);
    T at(size_t index);
    size_t size() const;
    bool empty() const;
    void clear();
}
