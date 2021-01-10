#include "shared/petools/petools.hpp"
#include "shared/memory/procedure.hpp"
#include "shared/logger/logger.hpp"

namespace shared::pe
{
	PPEB GetPEB()
	{
		__asm
		{
			mov eax, fs: [0x30]
		}
	}

	FARPROC GetProcAddress(HMODULE mod, std::string name)
	{
		auto modulebase = reinterpret_cast<uint32_t>(mod);

		PIMAGE_NT_HEADERS nthdr;

		if (!GetNTHeader(modulebase, &nthdr))
			return nullptr;

		auto opthdr = &nthdr->OptionalHeader;
		auto datadir = &opthdr->DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT];
		auto exportdir = reinterpret_cast<PIMAGE_EXPORT_DIRECTORY>(modulebase + datadir->VirtualAddress);

		auto ordtable = reinterpret_cast<uint16_t*>(modulebase + static_cast<uint32_t>(exportdir->AddressOfNameOrdinals));

		auto addrtable = reinterpret_cast<uint32_t*>(modulebase + static_cast<uint32_t>(exportdir->AddressOfFunctions));

		auto nametable = reinterpret_cast<uint32_t*>(modulebase + static_cast<uint32_t>(exportdir->AddressOfNames));

		for (uint16_t i = 0; i < exportdir->NumberOfNames; i++)
		{
			auto procname = reinterpret_cast<char*>(modulebase + nametable[i]);

			if (!procname)
				continue;

			if (name.find(procname) == 0)
			{
				return reinterpret_cast<FARPROC>(modulebase + addrtable[static_cast<uint16_t>(modulebase + ordtable[i])]);
			}
		}

		return nullptr;
	}

	HMODULE GetModuleHandles(std::string name)
	{
		auto pPEB = GetPEB();

		if (pPEB != nullptr)
		{
			auto InLoadOrderModuleList = &pPEB->LoaderData->InLoadOrderModuleList;

			for (auto entry = InLoadOrderModuleList->Flink; entry != InLoadOrderModuleList; entry = entry->Flink)
			{
				auto mod = CONTAINING_RECORD(entry, LDR_MODULE, InLoadOrderModuleList);

				auto name_s = memory::ToString(std::wstring(mod->BaseDllName.Buffer, mod->BaseDllName.Length));

				if (name.find(name_s.c_str()) == 0)
				{
					LOG(name, "found ->", std::hex, mod->BaseAddress);
					return static_cast<HMODULE>(mod->BaseAddress);
				}
			}
		}

		return nullptr;
	}
	
	bool GetNTHeader(DWORD_PTR mod, PIMAGE_NT_HEADERS* hdr)
	{
		auto pDosHdr = reinterpret_cast<PIMAGE_DOS_HEADER>(mod);

		if (!pDosHdr)
			return false;

		if (pDosHdr->e_magic != IMAGE_DOS_SIGNATURE)
			return false;

		auto pNTHdr = reinterpret_cast<PIMAGE_NT_HEADERS>(mod + pDosHdr->e_lfanew);

		if (pNTHdr->Signature != IMAGE_NT_SIGNATURE)
			return false;

		if (hdr)
			*hdr = pNTHdr;

		return true;
	}
}