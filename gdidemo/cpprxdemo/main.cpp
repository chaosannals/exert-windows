#include <iostream>
#include <rxcpp/rx.hpp>

int main() {
    auto keys = rxcpp::observable<>::create<int>(
    [](rxcpp::subscriber<int> dest) {
        for (;;) {
            int key = std::cin.get();
            dest.on_next(key);
        }
    }).
    publish();

    auto a = keys.
        filter([](int key) {return std::tolower(key) == 'a'; });

    auto g = keys.
        filter([](int key) {return std::tolower(key) == 'g'; });

    a.merge(g).
        subscribe([](int key) {
        std::cout << key << std::endl;
    });

    // run the loop in create
    keys.connect();
    return 0;
}
