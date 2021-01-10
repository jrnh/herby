#include "shared/console/console.hpp"

namespace shared::console
{
	bool Console::Create(std::string name)
	{
		if (!AllocConsole())
			return false;

		freopen_s(reinterpret_cast<_iobuf**>(stdin), "CONIN$", "r", stdin);
		freopen_s(reinterpret_cast<_iobuf**>(stdout), "CONOUT$", "w", stdout);
		freopen_s(reinterpret_cast<_iobuf**>(stderr), "CONOUT$", "w", stderr);

		GetStdHandle(STD_OUTPUT_HANDLE);

		return SetConsoleTitleA(name.c_str());
	}
	
	void Console::Destroy()
	{
		fclose(static_cast<_iobuf*>(stdin));
		fclose(static_cast<_iobuf*>(stdout));
		fclose(static_cast<_iobuf*>(stderr));

		FreeConsole();
	}
}