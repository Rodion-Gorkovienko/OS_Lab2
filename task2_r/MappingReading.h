#pragma once
#include <windows.h>
#include <iostream>
#include <string>

static void* Current_Mapping = nullptr;


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

void mapping_open(std::vector<std::string>* args) {
    auto map = OpenFileMapping(FILE_MAP_ALL_ACCESS, false, str2ws((*args)[0]).c_str());
    if (map == INVALID_HANDLE_VALUE || map == nullptr) {
        std::cout << "Mapping file not found. ERROR: " << GetLastError() << std::endl;
    }
    else {
        Current_Mapping = MapViewOfFile(map, FILE_MAP_ALL_ACCESS, 0, 0, 0);
        if (Current_Mapping == INVALID_HANDLE_VALUE) {
            std::cout << "File not mapped. ERROR: " << GetLastError() << std::endl;
        }
    }
}

void mapping_read(std::vector<std::string>* arg) {
    std::string data(reinterpret_cast<char*>(Current_Mapping));
    std::cout << "Read data:"<< std::endl << data << std::endl;
}

void mapping_close(std::vector<std::string>* arg) {
    Current_Mapping = nullptr;
}