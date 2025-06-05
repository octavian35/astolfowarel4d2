#include "Entry.h"
#include "../SDK/L4D2/Interfaces/IConVar.h"
#include "../SDK/L4D2/Interfaces/Models.h"
#include "../SDK/L4D2/Interfaces/IInput.h"
#include "../SDK/DirectX/DirectX.h"
void CGlobal_ModuleEntry::Load()
{
	while (!GetModuleHandleA("serverbrowser.dll"))
		std::this_thread::sleep_for(std::chrono::seconds(1));

	U::Offsets.Init();

	//Interfaces
	{
		I::BaseClient       = U::Interface.Get<IBaseClientDLL*>("client.dll", "VClient016");
		I::ClientEntityList = U::Interface.Get<IClientEntityList*>("client.dll", "VClientEntityList003");
		I::Prediction       = U::Interface.Get<IPrediction*>("client.dll", "VClientPrediction001");
		I::GameMovement     = U::Interface.Get<IGameMovement*>("client.dll", "GameMovement001");

		I::EngineClient     = U::Interface.Get<IVEngineClient*>("engine.dll", "VEngineClient013");
		I::EngineTrace      = U::Interface.Get<IEngineTrace*>("engine.dll", "EngineTraceClient003");
		I::EngineVGui       = U::Interface.Get<IEngineVGui*>("engine.dll", "VEngineVGui001");
		I::RenderView       = U::Interface.Get<IVRenderView*>("engine.dll", "VEngineRenderView013");
		I::DebugOverlay     = U::Interface.Get<IVDebugOverlay*>("engine.dll", "VDebugOverlay003");
		I::ModelInfo        = U::Interface.Get<IVModelInfo*>("engine.dll", "VModelInfoClient004");
		I::ModelRender      = U::Interface.Get<IVModelRender*>("engine.dll", "VEngineModel016");

		I::VGuiPanel        = U::Interface.Get<IVGuiPanel*>("vgui2.dll", "VGUI_Panel009");
		I::VGuiSurface      = U::Interface.Get<IVGuiSurface*>("vgui2.dll", "VGUI_Surface031");

		I::MatSystemSurface = U::Interface.Get<IMatSystemSurface*>("vguimatsurface.dll", "VGUI_Surface031");

		I::MaterialSystem   = U::Interface.Get<IMaterialSystem*>("materialsystem.dll", "VMaterialSystem080");
		I::Cvars	        = U::Interface.Get<ICvar*>("vstdlib.dll", "VEngineCvar007"); // 007 for l4d2, 004 for tf2 i believe.
		//Memory::GetInterface<ISteamClient017*>(gInts.steamclient, "SteamClient.dll", "SteamClient017");
		I::steamclient = U::Interface.Get<ISteamClient017*>("SteamClient.dll", "SteamClient017");
		/* steam */
		HSteamPipe hNewPipe = I::steamclient->CreateSteamPipe();
		HSteamUser hNewUser = I::steamclient->ConnectToGlobalUser(hNewPipe);

		I::steamfriends002 = reinterpret_cast<ISteamFriends002*>(I::steamclient->GetISteamFriends(hNewUser, hNewPipe, STEAMFRIENDS_INTERFACE_VERSION_002));
		I::steamuser = reinterpret_cast<ISteamUser017*>(I::steamclient->GetISteamUser(hNewUser, hNewPipe, STEAMUSER_INTERFACE_VERSION_017));
		I::Friends015 = reinterpret_cast<ISteamFriends015*>(I::steamclient->GetISteamFriends(hNewUser, hNewPipe, STEAMFRIENDS_INTERFACE_VERSION_015));
		I::Utils007 = reinterpret_cast<ISteamUtils007*>(I::steamclient->GetISteamUtils(hNewUser, STEAMUTILS_INTERFACE_VERSION_007));
		/* todo: CClientState E8 ? ? ? ? 8B 88 ? ? ? ? 41 engine.dll,this sig works/is valid. */
		/* i need to reverse CClientState doe.*/
		{
			I::ClientMode = **reinterpret_cast<void***>(U::Offsets.m_dwClientMode);
			XASSERT(I::ClientMode == nullptr);

			I::GlobalVars = **reinterpret_cast<CGlobalVarsBase***>(U::Offsets.m_dwGlobalVars);
			XASSERT(I::GlobalVars == nullptr);

			I::MoveHelper = **reinterpret_cast<IMoveHelper***>(U::Offsets.m_dwMoveHelper);
			XASSERT(I::MoveHelper == nullptr);

			I::Input = **reinterpret_cast<IInput_t***>(U::Offsets.m_dwIInput);
			XASSERT(I::Input == nullptr);

			g_dwDirectXDevice = **reinterpret_cast<void***>(U::Pattern.Find(_("shaderapidx9.dll"), _("A1 ? ? ? ? 8B 08 8B 51 ? 50 FF D2 8B F8")) + 0x1);
			//static HWND aWnd = FindWindowA(_("Valve001"), nullptr);

		}
	}

	G::Draw.Init();
	G::Hooks.Init();
}