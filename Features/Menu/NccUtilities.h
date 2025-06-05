#pragma once

#define NOMINMAX
#include <Windows.h>
#include <string>
#include <initializer_list>
#include "../../SDK/SDK.h"
#include "../../Util/Math/Vector/Vector.h"
#ifdef NDEBUG
#define strenc( s ) std::string( cx_make_encrypted_string( s ) )
#define charenc( s ) strenc( s ).c_str()
#define wstrenc( s ) std::wstring( strenc( s ).begin(), strenc( s ).end() )
#define wcharenc( s ) wstrenc( s ).c_str()
#else
#define strenc( s ) ( s )
#define charenc( s ) ( s )
#define wstrenc( s ) ( s )
#define wcharenc( s ) ( s )
#endif

#include <sstream> 
#include <iomanip>

namespace Utilities
{
	template <typename T>
	std::wstring to_wstring(const T a_value, const int precision = 6)
	{
		std::wostringstream out;
		out << std::setprecision(precision) << a_value;
		return out.str();
	}

	//DWORD GetModuleSize(char * ModuleName);
	//std::uint8_t* PatternScan(void* Module, const char* Signature);
	//int WaitForModules(std::int32_t Timeout, const std::initializer_list<std::wstring>& Modules);
}