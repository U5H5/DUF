// Name: [DUF] Discord User Fetcher | Github: U5H5 | File: memory.h
#pragma once
#include <windows.h>
#include <tlhelp32.h>
#include <iostream>
#include <string>

class m_memory
{
public:
    DWORD process_id;
    uintptr_t process_base;

    auto fetch_pid(const char* proc) -> DWORD
    {
        PROCESSENTRY32W e{ sizeof(e) };
        HANDLE handle = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
        std::wstring w(proc, proc + strlen(proc));

        if (Process32FirstW(handle, &e))
        {
            do
            {
                if (!_wcsicmp(e.szExeFile, w.c_str()))
                {
                    CloseHandle(handle);
                    return e.th32ProcessID;
                }
            } 
            while (Process32NextW(handle, &e));
        }

        CloseHandle(handle);
        return 0;
    }

    auto fetch_base(const char* process_name) -> uintptr_t
    {
        MODULEENTRY32W e{ sizeof(e) };
        HANDLE handle = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, process_id);
        std::wstring w(process_name, process_name + strlen(process_name));

        if (Module32FirstW(handle, &e))
        {
            do
            {
                if (!_wcsicmp(e.szModule, w.c_str()))
                {
                    CloseHandle(handle);
                    return (uintptr_t)e.modBaseAddr;
                }
            } 
            while (Module32NextW(handle, &e));
        }

        CloseHandle(handle);
        return 0;
    }

    auto read(uintptr_t base_offset, uintptr_t off1, uintptr_t off2) -> std::string
    {
        HANDLE handle = OpenProcess(PROCESS_VM_READ | PROCESS_QUERY_INFORMATION, FALSE, process_id);
        uintptr_t ptr = process_base + base_offset;
        uintptr_t tmp;
        SIZE_T br;
        char buf[33]{ NULL };

        for (auto off : { off1, off2 })
        {
            ReadProcessMemory(handle, (LPCVOID)ptr, &tmp, sizeof(tmp), &br);
            ptr = tmp + off;
        }

        ReadProcessMemory(handle, (LPCVOID)ptr, buf, sizeof(buf) - 1, &br);
        return std::string(buf);
    }

}; inline m_memory* memory = new m_memory();