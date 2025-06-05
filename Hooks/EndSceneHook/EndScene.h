#pragma once
#include "../Hooks.h"
#include "../../SDK/DirectX/DirectX.h"

namespace EndSceneHook
{
	inline Hook::CTable Table;
	using fn = HRESULT(__stdcall*)(IDirect3DDevice9*);
	HRESULT __stdcall Func(IDirect3DDevice9* pDevice);
	void Init();
}