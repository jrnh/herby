#pragma once

#include "shared/include/auto.hpp"
#include "shared/petools/peb.hpp"

namespace shared::pe
{
	PPEB GetPEB();
	FARPROC GetProcAddress(HMODULE mod, std::string name);
	HMODULE GetModuleHandles(std::string name);
	bool GetNTHeader(DWORD_PTR mod, PIMAGE_NT_HEADERS* hdr);
}