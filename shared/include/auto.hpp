#pragma once

#include "win32.hpp"

#include <iostream>
#include <chrono>
#include <algorithm>
#include <string>
#include <string_view>
#include <vector>
#include <atomic>
#include <future>
#include <mutex>
#include <thread>
#include <deque>
#include <fstream>
#include <map>
#include <unordered_map>
#include <any>
#include <filesystem>
#include <cstdlib>
#include <array>
#include <immintrin.h>
#include <cstdint>
#include <cstddef>
#include <utility>
#include <type_traits>

using namespace std::chrono_literals;
using namespace std::string_literals;

#define API_INLINE __inline
#define API_FORCEINLINE __forceinline

#define API_CDECL __cdecl
#define API_STDCALL __stdcall
#define API_THISCALL __thiscall
#define API_FASTCALL __fastcall

#define API_NT API_STDCALL
#define API_WIN32 API_STDCALL
#define API_D3D API_STDCALL
