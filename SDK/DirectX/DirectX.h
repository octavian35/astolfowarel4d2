#pragma once
#pragma warning (push)
#pragma warning (disable : 26495)
#include <d3d9.h>
//#include "d3dx9.h"
#pragma comment(lib, "d3d9.lib")
//#pragma comment(lib, "d3dx9.lib")
#pragma warning (pop)

inline void* g_dwDirectXDevice;
inline WNDPROC pOldWindowProc;
inline HWND hWindow;