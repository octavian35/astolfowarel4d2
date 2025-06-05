#pragma once

#include "../../SDK/SDK.h"

namespace Hooks
{
	namespace TerrorPlayer
	{
		namespace AvoidPlayers
		{
			inline Hook::CFunction Func;
			using FN = void(__fastcall*)(C_TerrorPlayer*, void*, CUserCmd*);

			void __fastcall Detour(C_TerrorPlayer* pThis, void* edx, CUserCmd* pCmd);
		}
		namespace CreateMove {
			inline Hook::CTable Table;
			using FN = bool(__fastcall*)(void*, void*, float, CUserCmd*);
			bool __fastcall Detour(void* ecx, void* edx, float input_sample_frametime, CUserCmd* cmd);
		}
		void Init();
	}
}