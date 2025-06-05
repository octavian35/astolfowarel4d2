#include "Hooks.h"
#include "EndSceneHook/EndScene.h"
using namespace Hooks;

void CGlobal_Hooks::Init()
{
	const MH_STATUS MH_INIT_STATUS = MH_Initialize();
	XASSERT(MH_INIT_STATUS != MH_STATUS::MH_OK);

	if (MH_INIT_STATUS == MH_STATUS::MH_OK)
	{
		BaseClient::Init();
		BasePlayer::Init();
		CL_Main::Init();
		ClientMode::Init();
		ClientPrediction::Init();
		EngineVGui::Init();
		Panels::Init();
		ModelRender::Init();
		ModelRenderSystem::Init();
		SequenceTransitioner::Init();
		TerrorGameRules::Init();
		TerrorPlayer::Init();
		WndProc::Initialize();
		EndSceneHook::Init();
	}

	XASSERT(MH_EnableHook(MH_ALL_HOOKS) != MH_STATUS::MH_OK);
}

void CGlobal_Hooks::Uninit() {
	WndProc::UnInitialize();
	MH_DisableHook(MH_ALL_HOOKS);
	MH_RemoveHook(MH_ALL_HOOKS);
}