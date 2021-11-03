#pragma once
#include <windows.h>
#include <string>

namespace MyConstants {
	const size_t KBsize{ 1024 };
	const size_t block_base{ 4096 };
	const size_t latin_al_size{ 26 };
	const size_t buff_size{ 256 };
	const size_t drive_path_size{ 4 };
}

std::string ProcArc2Str(WORD arc) {
	switch (arc)
	{
	case PROCESSOR_ARCHITECTURE_AMD64: return "AMD64";
	case PROCESSOR_ARCHITECTURE_ARM: return "ARM";
	case PROCESSOR_ARCHITECTURE_ARM64: return "ARM64";
	case PROCESSOR_ARCHITECTURE_IA64: return "Intel Itanium-based";
	case PROCESSOR_ARCHITECTURE_INTEL: return "x86";
	case PROCESSOR_ARCHITECTURE_UNKNOWN: return "Unknown architecture";
	default: return "Incorrect value";
	}
}

std::string RegionType2Str(DWORD arc) {
	switch (arc)
	{
	case MEM_IMAGE: return "mapped into the view of an image section";
	case MEM_MAPPED: return "mapped into the view of a section";
	case MEM_PRIVATE: return "private";
	default: return "incorrect value";
	}
}

std::string RegionState2Str(DWORD arc) {
	switch (arc)
	{
	case MEM_COMMIT: return "committed";
	case MEM_FREE: return "free pages";
	case MEM_RESERVE: return "reserved pages";
	default: return "incorrect value";
	}
}

static const std::vector<DWORD> Mempry_protect_list{
		{PAGE_EXECUTE},
		{PAGE_EXECUTE_READ},
		{PAGE_EXECUTE_READWRITE},
		{PAGE_EXECUTE_WRITECOPY},
		{PAGE_NOACCESS},
		{PAGE_READONLY},
		{PAGE_READWRITE},
		{PAGE_WRITECOPY},
		{PAGE_TARGETS_INVALID},
		{PAGE_GUARD},
		{PAGE_NOCACHE},
		{PAGE_WRITECOMBINE}
};

std::string MemProtectFlag2Str(DWORD arc) {
	switch (arc)
	{
	case PAGE_EXECUTE: return "Execute";
	case PAGE_EXECUTE_READ: return "Execute or read-only";
	case PAGE_EXECUTE_READWRITE: return "Execute, read-only, or read/write";
	case PAGE_EXECUTE_WRITECOPY: return "Execute, read-only, or copy-on-write";
	case PAGE_NOACCESS: return "None";
	case PAGE_READONLY: return "Read-only";
	case PAGE_READWRITE: return "Read-only or read/write";
	case PAGE_WRITECOPY: return "Read-only or copy-on-write";
	case PAGE_TARGETS_INVALID: return "Invalid targets";
	case PAGE_GUARD: return "Guard";
	case PAGE_NOCACHE: return "Non-cachable";
	case PAGE_WRITECOMBINE: return "Write-combined";
	default: return "Incorrect value";
	}
}

DWORD Str2ProtFlag(std::string attribute) {
	if (attribute == "PAGE_EXECUTE") return PAGE_EXECUTE;
	else if (attribute == "PAGE_EXECUTE_READ") return PAGE_EXECUTE_READ;
	else if (attribute == "PAGE_EXECUTE_READWRITE") return PAGE_EXECUTE_READWRITE;
	else if (attribute == "PAGE_NOACCESS") return PAGE_NOACCESS;
	else if (attribute == "PAGE_READONLY") return PAGE_READONLY;
	else if (attribute == "PAGE_READWRITE") return PAGE_READWRITE;
	else return 0;
}

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

void* str2ptr(const std::string& s)
{
	void* ptr;
	std::stringstream ss;
	ss << s;
	if (!(ss >> std::hex >> ptr)) {
		std::cout << ptr;
		ptr = nullptr;
	}
	return ptr;
}