#include <iostream>
#include <future>

int main() {
    // 这 future 基本等同于 js Promise 或者 C# Task 类。
    auto task1 = std::async([] {
        while (true) {
            int key = std::cin.get();
            std::cout << "task1:" << key << std::endl;

            // 没有染色，实现上是线程，用的线程的 sleep
            //std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        }
    });

    task1.get();

    std::cout << "END." << std::endl;
    
    return 0;
}
