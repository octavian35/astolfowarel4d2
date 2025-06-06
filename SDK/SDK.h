#pragma once

#include "DrawManager/DrawManager.h"
#define VK_0              0x30
#define VK_1              0x31
#define VK_2              0x32
#define VK_3              0x33
#define VK_4              0x34
#define VK_5              0x35
#define VK_6              0x36
#define VK_7              0x37
#define VK_8              0x38
#define VK_9              0x39

#define VK_A              0x41
#define VK_B              0x42
#define VK_C              0x43
#define VK_D              0x44
#define VK_E              0x45
#define VK_F              0x46
#define VK_G              0x47
#define VK_H              0x48
#define VK_J              0x49
#define VK_I              0x4A
#define VK_K              0x4B
#define VK_L              0x4C
#define VK_M              0x4D
#define VK_N              0x4E
#define VK_O              0x4F
#define VK_P              0x50
#define VK_Q              0x51
#define VK_R              0x52
#define VK_S              0x53
#define VK_T              0x54
#define VK_U              0x55
#define VK_V              0x56
#define VK_W              0x57
#define VK_X              0x58
#define VK_Y              0x59
#define VK_Z              0x5A
#include "Steam/ISteamClient017.h"
#include "Steam/ISteamFriends002.h"
#include "Steam/ISteamUser017.h"
#include "Steam/ISteamFriends015.h"
#include "Steam/ISteamUtils007.h"
namespace I { 
	inline void* ClientMode = nullptr;
	inline ISteamClient017* steamclient = nullptr;
	inline ISteamFriends002* steamfriends002 = nullptr;
	inline ISteamUser017* steamuser = nullptr;
	inline ISteamFriends015* Friends015 = nullptr;
	inline ISteamUtils007* Utils007 = nullptr;
}

struct WeaponSpawnInfo_t
{
	const wchar_t* m_szName;
	Color m_Color;
};

//Array of GetWeaponID's with their name and appropriate color.
static WeaponSpawnInfo_t g_aSpawnInfo[] =
{
	{ L"unknown", { 204, 204, 204, 255 } },
	{ L"pistol", { 204, 204, 204, 255 } },
	{ L"uzi", { 204, 204, 204, 255 } },
	{ L"pump shotgun", { 204, 204, 204, 255 } },
	{ L"auto shotgun", { 204, 204, 204, 255 } },
	{ L"m16a1", { 204, 204, 204, 255 } },
	{ L"hunting rifle", { 204, 204, 204, 255 } },
	{ L"mac10", { 204, 204, 204, 255 } },
	{ L"chrome shotgun", { 204, 204, 204, 255 } },
	{ L"scar", { 204, 204, 204, 255 } },
	{ L"military sniper", { 204, 204, 204, 255 } },
	{ L"spas", { 204, 204, 204, 255 } },
	{ L"firstaid", { 153, 255, 153, 255 } },
	{ L"molotov", { 255, 255, 255, 255 } },
	{ L"pipebomb", { 255, 255, 255, 255 } },
	{ L"pills", { 153, 255, 153, 255 } },
	{ L"gascan", { 255, 178, 0, 255 } },
	{ L"propane tank", { 255, 178, 0, 255 } },
	{ L"oxygen tank", { 255, 178, 0, 255 } },
	{ L"melee weapon", { 204, 204, 204, 255 } },
	{ L"chainsaw", { 204, 204, 204, 255 } },
	{ L"grenade launcher", { 204, 204, 204, 255 } },
	{ L"unknown", { 255, 255, 255, 255 } },
	{ L"adrenaline", { 153, 255, 153, 255 } },
	{ L"defibrillator", { 153, 255, 153, 255 } },
	{ L"vomitjar", { 255, 255, 255, 255 } },
	{ L"ak47", { 204, 204, 204, 255 } },
	{ L"unknown", { 255, 255, 255, 255 } },
	{ L"unknown", { 255, 255, 255, 255 } },
	{ L"firework crate", { 255, 255, 255, 255 } },
	{ L"incendiary ammo", { 255, 255, 255, 255 } },
	{ L"explosive ammo", { 255, 255, 255, 255 } },
	{ L"deagle", { 204, 204, 204, 255 } },
	{ L"mp5", { 204, 204, 204, 255 } },
	{ L"sg552", { 204, 204, 204, 255 } },
	{ L"awp", { 204, 204, 204, 255 } },
	{ L"scout", { 204, 204, 204, 255 } },
	{ L"m60", { 204, 204, 204, 255 } },
	{ L"unknown", { 255, 255, 255, 255 } },
};