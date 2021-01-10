#pragma once

#include "shared/include/auto.hpp"

namespace shared::memory
{

template< std::size_t N, typename T >
T vget( void* object )
{
	const auto procedure_array = *reinterpret_cast<std::uintptr_t**>( object );
	return reinterpret_cast<T>( procedure_array[ N ] );
}

template< std::size_t N, typename T, typename... ArgsT >
T Call( void* object, ArgsT... args )
{
	const auto procedure = vget< N, T( __thiscall* )( void*, ArgsT... ) >( object );
	return procedure( object, args... );
}

std::string ToLower(std::string data);
std::wstring ToLower(std::wstring data);

std::string ToUpper(std::string data);
std::wstring ToUpper(std::wstring data);

std::string ToString(std::wstring data);
std::wstring ToWString(std::string data);

template< typename T >
void Clamp(T& data, T min, T max)
{
	data = std::clamp(data, min, max);
}

template< typename T >
void Clamp(T* data, T min, T max)
{
	*data = std::clamp(*data, min, max);
}

}

#define VFUNC(return_type, name, index, args, raw) \
return_type name args \
{ \
	return shared::memory::Call<index, return_type>raw; \
}