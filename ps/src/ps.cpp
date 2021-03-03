#include <iostream>
#include "process.h"


int main()
{
    auto list = exert::list_processes();
    for (auto &a : list) {
        std::wcout << a.id << " | " << a.filename << std::endl;
    }
    return 0;
}

