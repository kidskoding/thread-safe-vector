#include <iostream>
#include "thread_safe_vector.hpp"

int main() {
    ThreadSafeVector<int> tsvec;

    tsvec.push_back(1);
    tsvec.push_back(2);
    tsvec.push_back(3);
    tsvec.push_back(4);
    tsvec.push_back(5);

    std::cout << tsvec << "\n";
}
