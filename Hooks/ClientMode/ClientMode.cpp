#include "ClientMode.h"

#include "../../Features/Vars.h"
#include "../../Features/EnginePrediction/EnginePrediction.h"
#include "../../Features/NoSpread/NoSpread.h"
#include "../../Features/Hitscan/Hitscan.h"
#include "../../Features/Misc/Misc.h"
#include "../../Features/Sequence Freezing/SequenceFreezing.h"
#include "../../Features/AutoShove/AutoShove.h"
using namespace Hooks;

bool __fastcall ClientMode::ShouldDrawFog::Detour(void* ecx, void* edx)
{
	return Table.Original<FN>(Index)(ecx, edx);
}

bool __fastcall ClientMode::CreateMove::Detour(void* ecx, void* edx, float input_sample_frametime, CUserCmd* cmd)
{
	if (!cmd || !cmd->command_number)
		return Table.Original<FN>(Index)(ecx, edx, input_sample_frametime, cmd);

	if (Table.Original<FN>(Index)(ecx, edx, input_sample_frametime, cmd))
		I::Prediction->SetLocalViewAngles(cmd->viewangles);
	DWORD pep;
	__asm mov pep, ebp;
	//bool* BSendPacket = reinterpret_cast<bool*>(*reinterpret_cast<char**>(pep) - 0x1D);

	C_TerrorPlayer* pLocal = I::ClientEntityList->GetClientEntity(I::EngineClient->GetLocalPlayer())->As<C_TerrorPlayer*>();

	if (pLocal && !pLocal->deadflag())
	{
		C_TerrorWeapon* pWeapon = pLocal->GetActiveWeapon()->As<C_TerrorWeapon*>();

		if (pWeapon)
		{
			if (Vars::Misc::Teleport)
			{
				if ((GetAsyncKeyState(Vars::Misc::TeleportKey.m_Var)) < 0) { // AirStuck with ViewAngles reset (((Teleport to 0, 0, 0))) (Reversed from some L4D2 hack)
					cmd->viewangles.x = 3.4028235e38;
					cmd->viewangles.y = 3.4028235e38;
					cmd->viewangles.z = 3.4028235e38;
					cmd->upmove = 3.4028235e38;
					cmd->forwardmove = 3.4028235e38;
					cmd->sidemove = 3.4028235e38;
				}
			}
			f::misc->run(pLocal, cmd); // run bhop before prediction for obv reasons
			F::EnginePrediction.Start(pLocal, cmd);
			{
				f::SequenceFreezing.Run(cmd, pLocal);
				f::hitscan.run(pLocal, cmd);
				f::autoshove.run(pLocal, cmd);
				F::NoSpread.Run(pLocal, pWeapon, cmd);
			}
			F::EnginePrediction.Finish(pLocal, cmd);
			viewangle = cmd->viewangles;
			//*BSendPacket = Vars::DT::Shifted == Vars::Exploits::SpeedHackValue;

		}
	}

	return false;
}

void __fastcall ClientMode::DoPostScreenSpaceEffects::Detour(void* ecx, void* edx, const void* pSetup)
{
	Table.Original<FN>(Index)(ecx, edx, pSetup);
}

float __fastcall ClientMode::GetViewModelFOV::Detour(void* ecx, void* edx)
{
	return Table.Original<FN>(Index)(ecx, edx);
}
#include "../../SDK/L4D2/Interfaces/IInput.h"
#include "../../SDK/L4D2/Interfaces/IConVar.h"
void __fastcall ClientMode::OverrideView::Detour(void* ecx, void* edx, void* View)
{
	return Table.Original<FN>(Index)(ecx, edx, View);

	
	//Table.Original<FN>(Index)(ecx, edx, View);
}
void ClientMode::Init()
{
	XASSERT(Table.Init(I::ClientMode) == false);
	XASSERT(Table.Hook(&ShouldDrawFog::Detour, ShouldDrawFog::Index) == false);
	XASSERT(Table.Hook(&CreateMove::Detour, CreateMove::Index) == false);
	XASSERT(Table.Hook(&DoPostScreenSpaceEffects::Detour, DoPostScreenSpaceEffects::Index) == false);
	XASSERT(Table.Hook(&GetViewModelFOV::Detour, GetViewModelFOV::Index) == false);
	XASSERT(Table.Hook(&OverrideView::Detour, OverrideView::Index) == false);
}