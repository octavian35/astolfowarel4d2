#include "TerrorPlayer.h"

#include "../../Features/Vars.h"

using namespace Hooks;

void __fastcall TerrorPlayer::AvoidPlayers::Detour(C_TerrorPlayer* pThis, void* edx, CUserCmd* pCmd)
{
	Func.Original<FN>()(pThis, edx, pCmd);
}


bool __fastcall TerrorPlayer::CreateMove::Detour(void* ecx, void* edx, float input_sample_frametime, CUserCmd* cmd) {
	return false;
}

void TerrorPlayer::Init()
{
	//AvoidPlayers
	{
		using namespace AvoidPlayers;

		const FN pfAvoidPlayers = reinterpret_cast<FN>(U::Offsets.m_dwAvoidPlayers);
		XASSERT(pfAvoidPlayers == nullptr);

		if (pfAvoidPlayers)
			XASSERT(Func.Init(pfAvoidPlayers, &Detour) == false);
	}

	{
		//using namespace CreateMove;
		//C_TerrorPlayer* pLocal = I::ClientEntityList->GetClientEntity(I::EngineClient->GetLocalPlayer())->As<C_TerrorPlayer*>();
		//if (!pLocal)
		//	return;
		//Table.Init(pLocal);
		//XASSERT(Table.Init(pLocal) == false);
		//XASSERT(Table.Hook(&CreateMove::Detour, 316) == false);


	}
}