#pragma once
#include "../SDK/L4D2/Includes/color.h"
#include "../SDK/SDK.h"
template <class T>
class CVar
{
public:
	T m_Var;
	const wchar_t* m_szDisplayName;
};

namespace Vars
{
	namespace Menu
	{
		inline bool Opened = false;
		//inline Rect_t Position = { 100, 100, 870, 480 };
		//inline const auto Title = L"Team Fortress 2";
		inline int TitleBarH = 16;

		inline int ShadowSize = 128;

		inline int SpacingX = 8;
		inline int SpacingY = 4;
		inline int SpacingText = 4;

		inline int CheckBoxW = 10;
		inline int CheckBoxH = 10;
		inline int CheckBoxFillSize = 4;

		inline int ButtonTabW = 198;
		inline int ButtonTabH = 25;

		inline int ButtonW = 90;
		inline int ButtonH = 20;

		inline int ButtonWSmall = 70;
		inline int ButtonHSmall = 16;

		inline int ComboBoxW = 90;
		inline int ComboBoxH = 14;

		inline int InputBoxW = 90;
		inline int InputBoxH = 14;
		inline int InputColorBoxW = 30;


		namespace Colors
		{
			//inline Color_t WindowBackground = { 30, 30, 30, 255 };
			inline Color Text = { 255, 255, 255, 190 };
			inline Color Widget = { 255, 255, 255, 190 };
			inline Color WidgetActive = { 240, 137, 229, 255 };
			inline Color ShadowColor = { 240, 137, 229, 255 };
			inline Color OutlineMenu = { 50, 50, 50, 255 };
		}
	}
	namespace Hitscan {
		inline bool bEnable = true;
		inline bool bAutoShoot = true;
		inline bool bKey = true;
		inline bool bSilentAim = false;
		inline int  iFov = 30;
		inline int  MaxPlayers = 2;
		inline int  iHitbox = 0; // 0 - Auto 1 - Head 2 - Body 3 - Pelvis 
		//inline int  Key; // R.
		inline CVar<int> AimKey{ VK_LSHIFT, L"Aim Key" };
		inline bool MultiPointIfNoHitboxesAreVisible;
		inline bool AimFovCircle = true;
		inline Color AimFov = {255, 255, 255, 255};
	}
	namespace Exploits {
		inline bool Speedhack = true;
		inline CVar<int> Key{ VK_CONTROL, L"SpeedHack" };
		inline int SpeedHackValue = 5;
	}
	namespace Misc {
		inline bool Namestealer = false;
		inline bool AutoStrafe = true;
		inline bool Bunnyhop = true;
		inline bool SequenceFreezing = true;
		inline bool DisableInterp = true;
		inline int SequenceFreezingValue = 0;
		//inline int  SequenceFreezingKey = 8; // R.
		inline CVar<int> Key{ VK_MENU, L"Sequence Freezing" };
		inline bool AutoShove = true;
		inline bool ThirdPerson = true;
		inline int ThirdPersonDistance = 100;
		inline CVar<int> ThirdPersonKey{ VK_B, L"Sequence Freezing" };
		inline bool Teleport = true;
		inline CVar<int> TeleportKey{ VK_P, L"Teleport Key" };

	}
	namespace Removals {
		inline bool NoSpread = true;
		inline bool NoRecoil = true;
		inline bool NoVisualRecoil = true; 
		inline bool BoomerVisual = true;
	}
	namespace ESP {
		inline bool Enabled = true;
		inline bool Box = true;
		inline bool Healthbar = true;
		inline bool EnemyOnly = true;
		inline bool Name = true;
		inline bool LocalESP = true;
		inline Color PlayerColor = { 240, 137, 229, 255 };
		inline Color PlayerInfectedColor = { 150, 15, 15, 255 };
		inline Color InfectedColor = { 150, 15, 15, 255 };
	}
	namespace Chams
	{
		inline bool Players = true;
		inline bool Infected = true;
		inline Color PlayerColor = { 240, 137, 229, 255};
		inline Color PlayerInfectedColor = { 150, 15, 15, 255 };
		inline Color InfectedColor = { 150, 15, 15, 255 };
	}
	namespace Ignore {
		inline bool IgnoreWitchUntilStartled = true;
		inline bool IgnoreFriends = true;
		inline bool IgnoreTeammates = true;
	}
	namespace DT
	{
		inline bool Enable = true;
		inline bool Recharging;
		inline bool shifting = false;
		inline int ticks = 0;
		inline int Shifted = 0;
	}
}