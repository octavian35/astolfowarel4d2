#pragma once
#include "../../SDK/SDK.h"
/* cancer namespace..? */
namespace Hooks 
{
	namespace Panels 
	{
		inline Hook::CTable Table;

		namespace PaintTraverse {
			constexpr uint32_t index = 41;
			//typedef void(__fastcall* PaintTraverse_t)(void*, void*, unsigned int, bool, bool);
			using FN = void(__fastcall*)(void*, void*, unsigned int, bool, bool);
			void __fastcall Detour(void* ecx, void* edx, unsigned int vguiPanel, bool forceRepaint, bool allowForce);
		}
		void Init();
	}
}