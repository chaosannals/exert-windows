#pragma once
#include<Windows.h>
#include<TlHelp32.h>
#include<Psapi.h>
#include<vector>
#include<memory>
#include<exert/base.h>
#include"disc.h"

namespace ext {

	struct process_t {
		DWORD id;
        DWORD pid;
        WCHAR filename[MAX_PATH];
        WCHAR imagename[MAX_PATH];
        WCHAR path[MAX_PATH];
	};

	std::vector<process_t> list_processes() {
        std::vector<logic_drive_t> dsv = list_disc();

        std::vector<process_t> result;
        handle_t sh = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
        
        if (sh == INVALID_HANDLE_VALUE) {
            return result;
        }

        PROCESSENTRY32W entry;
        entry.dwSize = sizeof(PROCESSENTRY32W);
        BOOL able = Process32FirstW(sh, &entry);
        while (able) {
            process_t one;
            one.id = entry.th32ProcessID;
            one.pid = entry.th32ParentProcessID;
            std::copy(entry.szExeFile, entry.szExeFile + MAX_PATH, one.filename);
            handle_t h = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, one.id);

            if (!GetProcessImageFileNameW(h, one.imagename, MAX_PATH)) {
                one.imagename[0] = '\0';
            }
            else {
                fill_path(dsv, one.imagename, one.path);
            }
            result.push_back(one);
            able = Process32NextW(sh, &entry);
        }
        return result;
	}

    // 通过名字找到进程。
    std::unique_ptr<process_t> find_process_by_name(const std::wstring& name) {
        auto processes = list_processes();
        for (auto p : processes) {
            if (name == p.filename) {
                return std::unique_ptr<process_t>(new process_t(p));
            }
        }
        return nullptr;
    }
}