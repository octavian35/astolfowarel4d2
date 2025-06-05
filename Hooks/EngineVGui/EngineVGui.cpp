#include "EngineVGui.h"

#include "../../Features/ESP/ESP.h"
#include "../../Features/Vars.h"
#include "../../SDK/L4D2/Interfaces/IConVar.h"
using namespace Hooks;

unsigned int __fastcall EngineVGui::GetPanel::Detour(void* ecx, void* edx, VGuiPanel_t type)
{
	return Table.Original<FN>(Index)(ecx, edx, type);
}

bool __fastcall EngineVGui::IsGameUIVisible::Detour(void* ecx, void* edx)
{
	return Table.Original<FN>(Index)(ecx, edx);
}

void __fastcall EngineVGui::ActivateGameUI::Detour(void* ecx, void* edx)
{
	Table.Original<FN>(Index)(ecx, edx);
}

void __fastcall EngineVGui::Paint::Detour(void* ecx, void* edx, int mode)
{
	Table.Original<FN>(Index)(ecx, edx, mode);

	if (!(mode & PAINT_UIPANELS))
		return;

	if (!G::Draw.m_nScreenW)
		G::Draw.m_nScreenW = I::BaseClient->GetScreenWidth();

	if (!G::Draw.m_nScreenH)
		G::Draw.m_nScreenH = I::BaseClient->GetScreenHeight();

	I::MatSystemSurface->StartDrawing();
	{
		if (Vars::Hitscan::AimFovCircle)
		{
			if (I::EngineClient->IsInGame())
			{
				//float flFOV = static_cast<float>(Vars::Visuals::FieldOfView.m_Var);
				static ConVar* fov = I::Cvars->FindVar("fov_desired");
				float FovDesired = fov->GetFloat();
				float flR = tanf(DEG2RAD(Vars::Hitscan::iFov) / 2.0f) / tanf(DEG2RAD(FovDesired) / 2.0f) * G::Draw.m_nScreenW;
				Color clr = Vars::Hitscan::AimFov;
				//clr.a() = static_cast<byte>(Vars::Visuals::AimFOVAlpha.m_Var);
				G::Draw.OutlinedCircle(G::Draw.m_nScreenW / 2, G::Draw.m_nScreenH / 2, flR, 68, clr);
			}
		}
		//Menu::Get().PaintTraverse();
		F::ESP.Render();

		//G::Draw.String(EFonts::DEBUG, 5, 5, { 204, 204, 204, 255 }, TXT_DEFAULT, _(L"Polonium - Left 4 dead 2 by Lak3 (unknowncheats.me)"));
	}
	I::MatSystemSurface->FinishDrawing();
}

void EngineVGui::Init()
{
	XASSERT(Table.Init(I::EngineVGui) == false);
	XASSERT(Table.Hook(&GetPanel::Detour, GetPanel::Index) == false);
	XASSERT(Table.Hook(&IsGameUIVisible::Detour, IsGameUIVisible::Index) == false);
	XASSERT(Table.Hook(&ActivateGameUI::Detour, ActivateGameUI::Index) == false);
	XASSERT(Table.Hook(&Paint::Detour, Paint::Index) == false);
}