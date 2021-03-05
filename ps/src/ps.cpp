#include <iostream>
#include "process.h"


int main()
{
    /*
    auto list = exert::list_processes();
    for (auto &a : list) {
        std::wcout << a.id << " | " << a.filename << std::endl;
        std::wcout << a.imagename << std::endl;
    }
    */
    auto dt = exert::find_process_by_name(L"DingTalk.exe");
    std::wcout << dt->filename << std::endl;
    return 0;
}

