#pragma once

#include "../../SDK/SDK.h"
inline Vector viewangle;
namespace Hooks
{
	namespace ClientMode
	{
		inline Hook::CTable Table;

		namespace OverrideView {
			using FN = void(__fastcall*)(void*, void*, void*);
			constexpr uint32_t Index = 19u;

			void __fastcall Detour(void* ecx, void* edx, void* View);
		}

		namespace ShouldDrawFog
		{
			using FN = bool(__fastcall*)(void*, void*);
			constexpr uint32_t Index = 18u;

			bool __fastcall Detour(void* ecx, void* edx);
		}

		namespace CreateMove
		{
			using FN = bool(__fastcall*)(void*, void*, float, CUserCmd*);
			constexpr uint32_t Index = 27u;

			bool __fastcall Detour(void* ecx, void* edx, float input_sample_frametime, CUserCmd* cmd);
		}

		namespace DoPostScreenSpaceEffects
		{
			using FN = void(__fastcall*)(void*, void*, const void*);
			constexpr uint32_t Index = 37u;

			void __fastcall Detour(void* ecx, void* edx, const void* pSetup);
		}

		namespace GetViewModelFOV
		{
			using FN = float(__fastcall*)(void*, void*);
			constexpr uint32_t Index = 40u;

			float __fastcall Detour(void* ecx, void* edx);
		}

		void Init();
	}
}