#include "Panels.h"
#include "../../Features/NewMenu/CMenu.h"
using namespace Hooks;

void __fastcall Panels::PaintTraverse::Detour(void* ecx, void* edx, unsigned int vguiPanel, bool forceRepaint, bool allowForce) 
{
	//oPaintTraverse(pPanels, edx, vguiPanel, forceRepaint, allowForce);
	Table.Original<FN>(index)(ecx, edx, vguiPanel, forceRepaint, allowForce);
	static unsigned int vguiFocusOverlayPanel;
	if (vguiFocusOverlayPanel == NULL)
	{
		const char* szName = I::VGuiPanel->GetName(vguiPanel);
		if (szName[0] == 'F' && szName[5] == 'O' && szName[12] == 'P')
		{
			vguiFocusOverlayPanel = vguiPanel;
		}
	}

	if (vguiFocusOverlayPanel == vguiPanel)
	{
		I::VGuiPanel->SetMouseInputEnabled(vguiPanel, g_Menu.menuOpen);
	}
}

void Panels::Init() {
	XASSERT(Table.Init(I::VGuiPanel) == false);
	XASSERT(Table.Hook(&PaintTraverse::Detour, PaintTraverse::index) == false);
}