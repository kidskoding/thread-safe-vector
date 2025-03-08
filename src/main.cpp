#include <iostream>

int main() {
    // std::cout << "hello world!" << "\n";

    std::vector<int> vec;
    vec.push_back(1);
    vec.pop_back();

    for(const auto& elem : vec) {
        std::cout << elem << " ";
    }
    std::cout << "\n";

    return 0;
}
