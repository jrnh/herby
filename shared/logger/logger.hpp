#pragma once

#include "shared/include/auto.hpp"

namespace shared::logger
{
	bool Create(std::string_view name);
	void Destroy();

	void Log(std::string_view format, ...);
}

#define LOG(data, ...) shared::logger::Log(data, __VA_ARGS__);