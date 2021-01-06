#pragma once
#include "shared/include/auto.hpp"

#ifdef _DEBUG
#define LOG_CREATE shared::logger::Logger::Create()
#define LOG_DESTROY shared::logger::Logger::Destroy()
#define LOG(...) shared::logger::Logger::Logger(__FUNCTION__, __VA_ARGS__)

namespace shared::logger
{
	class Logger
	{
	public:
		template< typename ... Args >
		explicit Logger(std::string function, Args ... to_print)
		{
			std::cout << '[' << function << "] ";
			Print(to_print ...);
		}

		static void Create()
		{
			AllocConsole();
			freopen("CON", "w", stdout);
		}

		static void Destroy()
		{
			std::cout << std::endl;
			FreeConsole();
		}
	private:
		template< typename T >
		static void Print(T format)
		{
			std::cout << format << std::endl;
		}

		template< typename T, typename ... Args >
		void Print(T current, Args ... next)
		{
			std::cout << current << ' ';
			Print(next...);
		}
	};
}

#else
#define LOG_CREATE
#define LOG_DESTROY
#define LOG
#endif
