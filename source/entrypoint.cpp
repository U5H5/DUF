// Name: [DUF] Discord User Fetcher | Github: U5H5 | File: entrypoint.cpp
#include "memory.h"
#include "offsets.h"

auto __fastcall main() -> int32_t
{
    SetConsoleTitleA("Discord User Fetcher");

    const char* target_exe = "Discord.exe";
    std::string username_final = "None";

    memory->process_id = memory->fetch_pid(target_exe);
    if (!memory->process_id)
    {
        std::cout << "[-] Failed to fetch Discord PID" << std::endl;
        std::cin.get();
    }
    std::cout << "[+] process_id: " << memory->process_id << std::endl;

    memory->process_base = memory->fetch_base(target_exe);
    if (!memory->process_base)
    {
        std::cout << "[-] Failed to fetch Discord BASE" << std::endl;
        std::cin.get();
    }
    std::cout << "[+] process_base: " << memory->process_base << std::endl;

    username_final = memory->read(offsets::username_base, offsets::username_offset_1, offsets::username_offset_2);
    if (username_final.empty())
    {
        std::cout << "[-] Failed to fetch Discord USER" << std::endl;
        std::cin.get();
    }

    std::cout << "[*] username: " << username_final << std::endl;

    std::cout << "[+] press enter to exit ..." << std::endl;

    std::cin.get();
}