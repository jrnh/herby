#include "csgo/engine.hpp"

void ImageQuit(bool synchronize)
{
	auto& console = console::Console::Instance();
	
	LOG_DESTROY;
#ifdef _DEBUG
	console.Destroy();
#endif // DEBUG


	if (synchronize)
		std::this_thread::sleep_for(1s);

	csgo::Destroy();
}

ULONG ImageFree(HMODULE image, unsigned long code = 0u)
{
	FreeLibraryAndExitThread(image, code);
	return code;
}

ULONG ImageLoad(core::Data* data)
{
	auto& system = win32::System::Instance();
	auto& console = console::Console::Instance();

	LOG_CREATE;
#ifdef _DEBUG
	console.Create();
#endif // DEBUG

	if (csgo::Create())
	{
		while (!system.Quit())
		{
			auto execute_state = system.GetExecuteState();

			switch (execute_state)
			{
				case win32::Load:
				{
				option::Load(system.GetExecuteDirectory());
				break;
				}
				case win32::Save:
				{
				option::Save(system.GetExecuteDirectory());
				break;
				}
			}

			std::this_thread::sleep_for(100ms);
		}

		ImageQuit(true);
	}

	return ImageFree(data->m_image, EXIT_SUCCESS);
}

bool GetImageDirectory(HMODULE image, char* output)
{
	char image_name_data[MAX_PATH] = { };
	if (!GetModuleFileNameA(image, image_name_data, MAX_PATH))
		return false;

	auto image_name = std::string(image_name_data);
	image_name = image_name.substr(0u, image_name.find_last_of('\\'));

	if (output)
	{
		std::memcpy(output, image_name.c_str(), image_name.size() * sizeof(char));
		return true;
	}

	return false;
}

bool GetImageDirectory(HMODULE image, wchar_t* output)
{
	wchar_t image_name_data[MAX_PATH] = { };
	if (!GetModuleFileNameW(image, image_name_data, MAX_PATH))
		return false;

	auto image_name = std::wstring(image_name_data);
	image_name = image_name.substr(0u, image_name.find_last_of(L'\\'));

	if (output)
	{
		std::memcpy(output, image_name.c_str(), image_name.size() * sizeof(wchar_t));
		return true;
	}

	return false;
}

void OnImageLoad(HMODULE image, void* reserved)
{
	auto data = reinterpret_cast<core::Data*>(reserved);

	if (data)
	{
		std::memcpy(&core::m_data, (const void*)((std::uintptr_t)data + 8), sizeof(core::Data));
	}
	else
	{
		data = &core::m_data;

		GetImageDirectory(image, core::m_data.m_loader_directory);
		GetImageDirectory(image, core::m_data.m_loader_directory_unicode);
	}

	core::m_data.m_image = image;

	DisableThreadLibraryCalls(image);
	CreateThread(nullptr, 0u, (LPTHREAD_START_ROUTINE)ImageLoad, data, 0u, nullptr);
}

void OnImageFree()
{
	ImageQuit(false);
}

BOOL WINAPI DllMain( HMODULE image, DWORD reason, LPVOID reserved )
{
	switch( reason )
	{
		case DLL_PROCESS_ATTACH:
		{
			OnImageLoad(image, reserved);
			break;
		}
		case DLL_PROCESS_DETACH:
		{
			OnImageFree();
			break;
		}
	}

	return TRUE;
}