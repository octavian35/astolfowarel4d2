#pragma once

#include "Math/Math.h"

#include <fstream>
#include <thread>
#include <vector>
#include <string>
#include <map>

inline std::wstring UTIL_GetVKeyName(const int vKey)
{
	switch (vKey)
	{
	case VK_LBUTTON:
		return L"Left Mouse";
	case VK_RBUTTON:
		return L"Right Mouse";
	case VK_MBUTTON:
		return L"Middle Mouse";
	case VK_XBUTTON1:
		return L"XButton 1";
	case VK_XBUTTON2:
		return L"XButton 2";
	case VK_DELETE:
		return L"Delete";
	case 0:
		return L"None";
	default:
		break;
	}

	wchar_t szBuff[16] = { L"\0" };

	if (GetKeyNameTextW(MapVirtualKeyW(vKey, MAPVK_VK_TO_VSC) << 16, szBuff, 16))
		return szBuff;

	return L"Unknown Key";
}


namespace Utils {
	inline bool IsKeyPressed(int i)
	{
		// "None", "Mouse 1", "Mouse 2", "Mouse 3", "Mouse 4", "Mouse 5", "Shift", "Alt", "R", "CTRL", "F"
		switch (i)
		{
		case 0: //None
			return true;
		case 1: //Mouse 1
			return GetAsyncKeyState(VK_LBUTTON);
		case 2:
			return GetAsyncKeyState(VK_RBUTTON);
		case 3:										  //Mouses 1-5
			return GetAsyncKeyState(VK_MBUTTON);
		case 4:
			return GetAsyncKeyState(VK_XBUTTON1);
		case 5:
			return GetAsyncKeyState(VK_XBUTTON2);
		case 6: //Shift
			return GetAsyncKeyState(VK_SHIFT);
		case 7: //Alt
			return GetAsyncKeyState(VK_MENU);
		case 8: //R
			return GetAsyncKeyState(0x52);
		case 9: // CTRL
			return GetAsyncKeyState(VK_CONTROL);
		case 10: //F
			return GetAsyncKeyState(0x46);
		case 11: // B
			return ( GetAsyncKeyState('B') );
		default:
			return false;
		}

		return false;
	}
}