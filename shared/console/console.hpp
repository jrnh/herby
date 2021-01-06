#pragma once

#include "shared/include/auto.hpp"
#include "shared/detail/singleton.hpp"

namespace shared::console
{
	class Console : public detail::Singleton < Console >
	{
	public:
		bool Create(std::string name = "herby");
		void Destroy();
	public:
		template < typename ... Args >
		void Log(Args ... to_print)
		{
			time_t m_time = time(nullptr);
			std::string time_str = ctime(&m_time);
			time_str.pop_back();

			std::cout << time_str << ": ";

			Print(to_print ...);
		}
	public:
		template <typename T >
		void Print(T format)
		{
			std::cout << format << std::endl;
		}

		template <typename T, typename ... Args >
		void Print(T current, Args... next)
		{
			std::cout << current << ' ';
			Print(next...);
		}
	};
}