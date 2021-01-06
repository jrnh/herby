#pragma once

#if !defined( WIN32_LEAN_AND_MEAN )
#define WIN32_LEAN_AND_MEAN
#endif // WIN32_LEAN_AND_MEAN

#if !defined( NOMINMAX )
#define NOMINMAX
#endif // NOMINMAX

#if !defined(_CRT_SECURE_NO_WARNINGS)
#define _CRT_SECURE_NO_WARNINGS
#endif // _CRT_SECURE_NO_WARNINGS

#include <windows.h>
#include <winternl.h>
#include <intrin.h>

#pragma warning( push )
#pragma warning( disable : 4005 )
#include <ntstatus.h>
#pragma warning( pop )

#include <d3d9.h>
#pragma comment( lib, "d3d9.lib" )