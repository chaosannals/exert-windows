#include <iostream>
#include "process.h"
#include "disc.h"

int main()
{
    auto dlist = list_disc();
    for (auto& d : dlist) {
        std::wcout << d.name << std::endl;
    }
    std::wcout << dlist.size() << std::endl;

    /*
    auto list = ext::list_processes();
    for (auto &a : list) {
        std::wcout << a.id << " | " << a.filename << std::endl;
        std::wcout << a.imagename << std::endl;
    }
    */
    
    /*
    auto dt = exert::find_process_by_name(L"DingTalk.exe");
    std::wcout << dt->filename << std::endl;
    */
    return 0;
}

