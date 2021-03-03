#pragma once
#include<Windows.h>
#include<TlHelp32.h>
#include<vector>

namespace exert {

    template<typename T> class closer_t {
        const T handle;
    public:
        closer_t(T handle):handle(handle) {}
        closer_t(const closer_t&) = delete;
        ~closer_t() { CloseHandle(handle); }
        operator T() { return handle; }
    };



	struct process_t {
		DWORD id;
        WCHAR filename[MAX_PATH];
	};

	std::vector<process_t> list_processes() {
        std::vector<process_t> result;
        closer_t<HANDLE> sh = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
        
        if (sh == INVALID_HANDLE_VALUE) {
            return result;
        }

        PROCESSENTRY32W entry;
        entry.dwSize = sizeof(PROCESSENTRY32W);
        BOOL able = Process32FirstW(sh, &entry);
        while (able) {
            process_t one;
            one.id = entry.th32ProcessID;
            std::copy(entry.szExeFile, entry.szExeFile + MAX_PATH, one.filename);
            result.push_back(one);
            able = Process32NextW(sh, &entry);
        }
        return result;
	}
}