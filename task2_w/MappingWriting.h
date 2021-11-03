#pragma once
#include <iostream>
#include <string>
#include <windows.h>

static void* Current_Mapping = nullptr;
static HANDLE Current_File = INVALID_HANDLE_VALUE;

std::wstring str2ws(const std::string& s)
{
    int len;
    int slength = (int)s.length() + 1;
    len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0);
    std::wstring buf;
    buf.resize(len);
    MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength,
        const_cast<wchar_t*>(buf.c_str()), len);
    return buf;
}

void mapping_crate(std::vector<std::string>* args) {

    Current_File = CreateFile(str2ws((*args)[0]).c_str(), GENERIC_READ | GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, 0, nullptr);
    if (Current_File == INVALID_HANDLE_VALUE) {
        std::cout << "File not created. ERROR: " << GetLastError() << std::endl;
        return;
    }

    auto new_map = CreateFileMapping(Current_File, nullptr, PAGE_READWRITE, 0, 128, str2ws((*args)[1]).c_str());
    if (!new_map) {
        std::cout << "Mapping file not created. ERROR: " << GetLastError() << std::endl;
        return;
    }

    Current_Mapping = MapViewOfFile(new_map, FILE_MAP_ALL_ACCESS, 0, 0, 128);
    if (Current_Mapping == INVALID_HANDLE_VALUE) {
        CloseHandle(Current_File);
        std::cout << "File not mapped. ERROR: " << GetLastError() << std::endl;
        return;
    }
    std::cout << "File mapped successfully." << std::endl;
}

void mapping_write(std::vector<std::string>* args) {
    memcpy(Current_Mapping, (*args)[2].data(), (*args)[2].size());
    std::cout << "Data written to projected file successfully." << std::endl;
}

void mapping_delete(std::vector<std::string>*) {
    UnmapViewOfFile(Current_Mapping);
    CloseHandle(Current_File);
    std::cout << "Mapped file closed" << std::endl;
}