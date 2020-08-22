#pragma once
#include <Windows.h>

namespace winapi_wrappers {
	using handle_t = HANDLE;
	using arg_t = void*;
	using ret_t = DWORD;
	using id_t = DWORD;
	using bool_t = BOOL;
	using atomic32_t = volatile unsigned long;
}