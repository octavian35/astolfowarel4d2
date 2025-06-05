#include "Menu.h"
#include "GUI.h"
#include "../Vars.h"

/*
* ToDo for this cheat. 
* NameStealer (cathook one?)
* Sequence Freezing
* CL_Sendmove Rebuild.. Not really needed..?
* Charger Free turn/move turn.
* AutoPistol
*/

void Menu::Init()
{
	m_isVisible = false;

	CWindow Main(100, 100, 700, 300);
	auto AimbotTab = new CChild(0, 0, 220, L"Hitscan");
	{
		AimbotTab->AddControl(new CCheckBox(L"Enabled", &Vars::Hitscan::bEnable));
		AimbotTab->AddControl(new CCheckBox(L"Silent", &Vars::Hitscan::bSilentAim));
		AimbotTab->AddControl(new CSlider<int>(L"Field of View", 0, 180, &Vars::Hitscan::iFov));
		AimbotTab->AddControl(new CCheckBox(L"Autoshoot", &Vars::Hitscan::bAutoShoot));
		AimbotTab->AddControl(new CCheckBox(L"Key?", &Vars::Hitscan::bKey));
	}
	Main.AddControl(AimbotTab);
	auto ESPTab = new CChild(0, 100, 220, L"ESP");
	{
		ESPTab->AddControl(new CCheckBox(L"Enabled", &Vars::ESP::Enabled));
		ESPTab->AddControl(new CCheckBox(L"Box", &Vars::ESP::Box));
		ESPTab->AddControl(new CCheckBox(L"Healthbar", &Vars::ESP::Healthbar));
		ESPTab->AddControl(new CCheckBox(L"Name", &Vars::ESP::Name));
		ESPTab->AddControl(new CCheckBox(L"Class", &Vars::ESP::Class));
		ESPTab->AddControl(new CCheckBox(L"Conditions", &Vars::ESP::Conditions));
	}
	Main.AddControl(ESPTab);
	auto Removals = new CChild(230, 0, 220, L"Removals");
	{
		Removals->AddControl(new CCheckBox(L"NoSpread", &Vars::Removals::NoSpread));
		Removals->AddControl(new CCheckBox(L"NoRecoil", &Vars::Removals::NoRecoil));
		Removals->AddControl(new CCheckBox(L"No Visual Recoil", &Vars::Removals::NoVisualRecoil));
		//Removals->AddControl(new CCheckBox(L"No Flash", &Vars::Misc::NoFlash));
		//Removals->AddControl(new CCheckBox(L"No Smoke", &Vars::Misc::NoSmoke));
	}
	Main.AddControl(Removals);
	auto MiscAgainTab = new CChild(460, 0, 220, L"Ignores");
	{
		MiscAgainTab->AddControl(new CCheckBox(L"Ignore Friends", &Vars::Ignore::IgnoreFriends));
		MiscAgainTab->AddControl(new CCheckBox(L"Ignore Witch Until Startled", &Vars::Ignore::IgnoreWitchUntilStartled));
	}
	Main.AddControl(MiscAgainTab);

	auto MiscTab = new CChild(460, 55, 220, L"Misc");
	{
		MiscTab->AddControl(new CCheckBox(L"BunnyHop", &Vars::Misc::Bunnyhop));
		MiscTab->AddControl(new CCheckBox(L"AutoStrafe", &Vars::Misc::AutoStrafe));
		MiscTab->AddControl(new CCheckBox(L"Sequence Freezing", &Vars::Misc::SequenceFreezing));
		MiscTab->AddControl(new CSlider<int>(L"Value", 0, 300, &Vars::Misc::SequenceFreezingValue));
		MiscTab->AddControl(new CCheckBox(L"SpeedHack", &Vars::Exploits::Speedhack));
	}
	Main.AddControl(MiscTab);

	MenuForm.AddWindow(Main);

}
void Menu::Kill()
{}

void Menu::PaintTraverse()
{
	if (m_isVisible)
		MenuForm.Paint();
}

void Menu::Click()
{
	if (m_isVisible)
		MenuForm.Click();
}

void Menu::HandleInput(WPARAM vk)
{
	if (m_isVisible)
		MenuForm.HandleInput(vk);
}

void Menu::Toggle()
{
	m_isVisible = !m_isVisible;
}