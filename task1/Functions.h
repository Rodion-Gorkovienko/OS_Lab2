#pragma once
#include <windows.h>
#include <iostream>
#include <sstream>
#include "Convertation.h"
#include <bitset>
#include <limits>

std::string* func_gets_secure() {
    std::string* str = new std::string;
    do {
        system("cls");
        std::cout << "Enter data ";
        if (!(std::cin >> *str) || (*str)[0] == '\0' || (*str)[0] == EOF) {
            str->clear();
            std::cout << "Something wrong. Try again." << std::endl;
        }
        else if ((*str)[0] == '\n') {
            str->clear();
            std::cout << "String is empty. Try again." << std::endl;
        }
        else for (char& c : *str) {
            if (c == '\n')
                c = '\0';
        }
    } while (str->size() == 0);
    return str;
}

int getnum() {
    int i;
    while (!(std::cin >> i))
        std::cout << "Invalid input. Try again:" << std::endl;
    return i;
}

void get_system_info(std::vector<std::string>*) {
    SYSTEM_INFO sys_inf;
    GetSystemInfo(&sys_inf);

    std::string proc_mask = std::bitset<32>(sys_inf.dwActiveProcessorMask).to_string();
    for (unsigned char i = 1; i < 4; i++) { proc_mask.insert(9 * i - 1, " "); }

    std::cout << "Processor architecture: " << ProcArc2Str(sys_inf.wProcessorArchitecture) << std::endl;
    std::cout << "Number of processors: " << sys_inf.dwNumberOfProcessors << std::endl;
    std::cout << "Active processor mask: " << proc_mask << std::endl;
    std::cout << "Page size: " << sys_inf.dwPageSize << std::endl;
    std::cout << "Minimum application address: " << sys_inf.lpMinimumApplicationAddress << std::endl;
    std::cout << "Maximum application address: " << sys_inf.lpMaximumApplicationAddress << std::endl;
    std::cout << "Processor level: " << sys_inf.wProcessorLevel << std::endl;
    std::cout << "Processor revision: " << sys_inf.wProcessorRevision << std::endl;
    std::cout << "Allocation Granularity: " << sys_inf.dwAllocationGranularity << std::endl;

}

void get_mem_status(std::vector<std::string>*) {
    MEMORYSTATUSEX status;
    status.dwLength = sizeof(status);
    if (GlobalMemoryStatusEx(&status)) {
        std::cout << "Memory load: " << status.dwMemoryLoad << "%" << std::endl;
        std::cout << "Available phys memory: " << status.ullAvailPhys / MyConstants::KBsize << "/" << status.ullTotalPhys / MyConstants::KBsize << " Kb" << std::endl;
        std::cout << "Available page memory: " << status.ullAvailPageFile / MyConstants::KBsize << "/" << status.ullTotalPageFile / MyConstants::KBsize << " Kb" << std::endl;
        std::cout << "Available virtual memory: " << status.ullAvailVirtual / MyConstants::KBsize << "/" << status.ullTotalVirtual / MyConstants::KBsize << " Kb" << std::endl;
    }
    else {
        std::cout << "Global memory status not received. ERROR: " << GetLastError() << std::endl;
    }
}

void print_protection_type(DWORD& protect) {
    std::cout << "Memory protection: " << std::endl;
    for (const auto& flag : Mempry_protect_list) {
        if (flag & protect) {
            std::cout << "- " << MemProtectFlag2Str(flag) << std::endl;
            return;
        }
    }
}

void get_reg_status(std::vector<std::string>* arg) {
    MEMORY_BASIC_INFORMATION info;

    void* ptr = str2ptr((*arg)[0]);
    if (ptr == nullptr) {
        std::cout << "Invalid input." << std::endl;
        return;
    }
    if (VirtualQuery(ptr, &info, sizeof(info))) {
        std::cout << "Base address: " << info.BaseAddress << std::endl;
        std::cout << "Allocation base address: " << info.AllocationBase << std::endl;
        std::cout << "Region size: " << info.RegionSize << std::endl;
        std::cout << "Type: pages within region are " << RegionType2Str(info.Type) << std::endl;
        std::cout << "State: " << RegionState2Str(info.State) << std::endl;
        if (info.Protect)
            print_protection_type(info.Protect);
    }
    else {
        std::cout << "Information about the region was not received. ERROR: " << GetLastError();
        return;
    }
}

void reserv(void* ptr, const bool& phys) {
    SYSTEM_INFO info;
    GetSystemInfo(&info);

    DWORD type = MEM_RESERVE | ((phys) ? MEM_COMMIT : 0);
    void* res_ptr = nullptr;

    int pages = 0;
    do {
        std::cout << "Enter amount of pages to reserv:" << std::endl;
        pages = getnum();
        if (pages < 1) {
            std::cout << " amount should be at least 1" << std::endl;
        }
    } while (pages < 1);

    res_ptr = VirtualAlloc(ptr, pages * info.dwPageSize, type, PAGE_EXECUTE_READWRITE);
    if (!res_ptr) {
        std::cout << "ERROR: " << GetLastError();
        return;
    }
    std::cout << "Reserved at" << res_ptr << std::endl;

}

void auto_reserv(std::vector<std::string>*) {
    reserv(nullptr, false);
}

void auto_reserv_phys(std::vector<std::string>*) {
    reserv(nullptr, true);
}

void region_reserv(std::vector<std::string>* arg) {
    void* ptr = str2ptr((*arg)[0]);
    if (ptr == nullptr) {
        std::cout << "Invalid input." << std::endl;
        return;
    }
    reserv(ptr, false);
}

void region_reserv_phys(std::vector<std::string>* arg) {
    void* ptr = str2ptr((*arg)[0]);
    if (ptr == nullptr) {
        std::cout << "Invalid input." << std::endl;
        return;
    }
    reserv(ptr, true);
}

void region_protection(std::vector<std::string>* arg) {
    SYSTEM_INFO info;
    GetSystemInfo(&info);

    DWORD old_protect = 0;
    DWORD new_protect = Str2ProtFlag((*arg)[1]);
    void* ptr = str2ptr((*arg)[0]);
    if (ptr == nullptr) {
        std::cout << "Invalid input." << std::endl;
        return;
    }

    if (VirtualProtect(ptr, info.dwPageSize, new_protect, &old_protect)) {
        std::cout << "Protection changed successfully." << std::endl;
        std::cout << "Old protection: " << std::endl;
        print_protection_type(old_protect);
        std::cout << "New protection : " << std::endl;
        print_protection_type(new_protect);
    }
    else
        std::cout << "Protection flags are not set. ERROR: " << GetLastError() << std::endl;
}

void region_write(std::vector<std::string>* arg) {
    std::string *data;
    void* ptr = str2ptr((*arg)[0]);
    if (ptr == nullptr) {
        std::cout << "Invalid input." << std::endl;
        return;
    }
    std::cout << "Enter data ";
    data = func_gets_secure();

    memcpy(ptr, data->data(), data->size());

    std::cout << "Data successfully entered: " << *data << std::endl;
}

void region_read(std::vector<std::string>* arg) {
    SYSTEM_INFO info;
    GetSystemInfo(&info);

    std::string data;
    void* ptr = str2ptr((*arg)[0]);
    if (ptr == nullptr) {
        std::cout << "Invalid input." << std::endl;
        return;
    }

    data.assign(reinterpret_cast<char*>(ptr), info.dwPageSize);
    std::cout << "Data successfully read: " << data << std::endl;
}

void region_free(std::vector<std::string>* arg) {
    void* ptr = str2ptr((*arg)[0]);
    if (ptr == nullptr) {
        std::cout << "Invalid input." << std::endl;
        return;
    }

    if (!VirtualFree(ptr, 0, MEM_RELEASE)) {
        std::cout << "ERROR: " << GetLastError() << std::endl;
        return;
    }
    std::cout << "Memory freed successfully\n";
}