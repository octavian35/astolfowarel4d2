#include "ESP.h"
#include "../../SDK/L4D2/Interfaces/IInput.h"
#include "../Vars.h"

const char* GetZombieName(int index) {
	switch (index) {
		case Boomer: {
			return "Boomer";
			break;
		}
		case Smoker: {
			return "Smoker";
			break;
		}
		case Hunter: {
			return "Hunter";
			break;
		}
		case Jockey: {
			return "Jockey";
			break;
		}
		case Spitter: {
			return "Spitter";
			break;
		}
		case Charger: {
			return "Charger";
			break;
		}
		case Tank: {
			return "Tank";
			break;
		}
		case Witch: {
			return "Witch";
			break;
		}
		default:
			break;
	}
	return "Fuck niggers.";
}

void CFeatures_ESP::Render()
{
	if (!I::EngineClient->IsInGame() || I::EngineVGui->IsGameUIVisible())
		return;
	if (!Vars::ESP::Enabled)
		return;
	const int nLocalIndex = I::EngineClient->GetLocalPlayer();

	C_TerrorPlayer* pLocal = I::ClientEntityList->GetClientEntity(nLocalIndex)->As<C_TerrorPlayer*>();

	if (!pLocal)
		return;

	player_info_t pi;

	int x, y, w, h;
	for (int n = 1; n < (I::ClientEntityList->GetMaxEntities() + 1); n++)
	{
		if (n == nLocalIndex)
			continue;
		

		IClientEntity* pEntity = I::ClientEntityList->GetClientEntity(n);

		if (!pEntity || pEntity->IsDormant())
			continue;

		ClientClass* pCC = pEntity->GetClientClass();

		if (!pCC)
			continue;
		
		switch (pCC->m_ClassID)
		{
			case CTerrorPlayer:
			case SurvivorBot:
			{
				C_TerrorPlayer* pPlayer = pEntity->As<C_TerrorPlayer*>();

				if (pPlayer->deadflag() || !GetBounds(pPlayer, x, y, w, h))
					break;

				const int nDrawX = x + (w / 2);
				int nDrawY = y + (h / 2);

				const int nHealth = pPlayer->GetActualHealth(); //Returns health including the buffer from pills etc.
				const int nMaxHealth = pPlayer->GetMaxHealth();
				const int newHealth = std::clamp(nHealth, 0, nMaxHealth);
				const bool bIsSurvivor = (pPlayer->GetTeamNumber() == TEAM_SURVIVOR);
				int distance = U::Math.GetDistanceMeters(pLocal->GetAbsOrigin(), pPlayer->GetAbsOrigin());

				const Color clrHealth = G::Util.GetHealthColor(nHealth, nMaxHealth);
													// if it is survivor       if not 
				const Color clrTeam = bIsSurvivor ? Vars::ESP::PlayerColor : Vars::ESP::PlayerInfectedColor;

				/* box */
				if (Vars::ESP::Box) {
					G::Draw.OutlinedRect(x - 1, y - 1, w + 2, h + 2, Color(0, 0, 0, 255));
					G::Draw.OutlinedRect(x, y, w, h, clrTeam);
					G::Draw.OutlinedRect(x + 1, y + 1, w - 2, h - 2, Color(0, 0, 0, 255));
				}
				if (Vars::ESP::Name) {
					if (I::EngineClient->GetPlayerInfo(n, &pi))
					{
						G::Draw.String(EFonts::ESP_NAME, x + (w / 2),
							y - G::Draw.GetFontHeight(EFonts::ESP_NAME) - 1,
							clrTeam,
							TXT_CENTERX,
							pi.name);
						//G::Draw.String(EFonts::ESP_NAME, nDrawX, nDrawY, clrTeam, TXT_CENTERXY, pi.name);
						//nDrawY += G::Draw.GetFontHeight(EFonts::ESP_NAME);
					}
					std::string meter = "[" + std::to_string(distance) + "M]";
					G::Draw.String(EFonts::METER_THING, x + (w / 2),
						y + h + 2, // 2-pixel offset below the box
						{255,255,255,255},
						TXT_CENTERX,
						meter.c_str());
				}
				// helth
				if (Vars::ESP::Healthbar) {
					float flHealth = static_cast<float>(newHealth);
					float flMaxHealth = static_cast<float>(nMaxHealth);
					int nHeight = (h + (flHealth < flMaxHealth ? 2 : 1));
					int nHeight2 = (h + 2);
					float ratio = (flHealth / flMaxHealth);
					G::Draw.Rect(x - 6, y - 1, 4, h + 2, Color(0, 0, 0, 255));
					G::Draw.Rect(x - 5, (y + nHeight - (nHeight * ratio)), 2, (nHeight * ratio) - 1, Color(0, 255, 0, 255));
				}
				/*
				if (bIsSurvivor)
				{
					C_BaseCombatWeapon* pWeapon = pPlayer->GetActiveWeapon();

					if (pWeapon)
					{
						G::Draw.String(EFonts::ESP_WEAPON, nDrawX, nDrawY, { 204, 204, 204, 255 }, TXT_CENTERXY, pWeapon->GetPrintName());
						nDrawY += G::Draw.GetFontHeight(EFonts::ESP_WEAPON);
					}
				}
				*/
				break;
			}
			case CWeaponSpawn:
			{
				C_WeaponSpawn* pSpawn = pEntity->As<C_WeaponSpawn*>();

				if (!GetBounds(pSpawn, x, y, w, h))
					break;

				const int nID = U::Math.Clamp(pSpawn->GetWeaponID(), 0, 38);
				G::Draw.String(EFonts::ESP_NAME, x + (w / 2), y + (h / 2), g_aSpawnInfo[nID].m_Color, TXT_CENTERXY, g_aSpawnInfo[nID].m_szName);

				break;
			}
			case CPropMountedGun:
			case CPropMinigun:
			{
				C_BaseMountedWeapon* pMounted = pEntity->As<C_BaseMountedWeapon*>();

				if (!GetBounds(pMounted, x, y, w, h))
					break;

				const int nDrawX = x + (w / 2);
				int nDrawY = y + (h / 2);

				G::Draw.String(EFonts::ESP_NAME, nDrawX, nDrawY, { 204, 204, 204, 255 }, TXT_CENTERXY, L"mounted weapon");
				nDrawY += G::Draw.GetFontHeight(EFonts::ESP_NAME);

				G::Draw.String(EFonts::ESP_NAME, nDrawX, nDrawY, { 204, 204, 204, 255 }, TXT_CENTERXY, L"heat: %.1f", U::Math.Clamp(pMounted->m_heat() * 100.0f, 0.0f, 100.0f));
				nDrawY += G::Draw.GetFontHeight(EFonts::ESP_NAME);

				if (pMounted->m_overheated())
				{
					G::Draw.String(EFonts::ESP_NAME, nDrawX, nDrawY, { 240, 230, 140, 255 }, TXT_CENTERXY, L"OVERHEATED");
					nDrawY += G::Draw.GetFontHeight(EFonts::ESP_NAME);
				}

				break;
			}
			case Infected: {
				C_Infected* pInfected = pEntity->As<C_Infected*>();
				if (!GetBounds(pInfected, x, y, w, h) || !pInfected)
					break;
				if (!G::Util.IsInfectedAlive(pInfected->m_usSolidFlags(), pInfected->m_nSequence(), pCC->m_ClassID))
				{
					continue;
				}
				const Color color = Vars::ESP::InfectedColor;
				int distance = U::Math.GetDistanceMeters(pLocal->GetAbsOrigin(), pInfected->GetAbsOrigin());

				//if (pInfected->GetActualHealth() == 0 || pInfected->GetHealth() == 0)
				//	continue;
				const int nDrawX = x + (w / 2);
				int nDrawY = y + (h / 2);
				if (Vars::ESP::Box) {
					G::Draw.OutlinedRect(x - 1, y - 1, w + 2, h + 2, Color(0, 0, 0, 255));
					G::Draw.OutlinedRect(x, y, w, h, color);
					G::Draw.OutlinedRect(x + 1, y + 1, w - 2, h - 2, Color(0, 0, 0, 255));
				}
				if (Vars::ESP::Name) {
					G::Draw.String(EFonts::ESP_NAME, x + (w / 2),
						y - G::Draw.GetFontHeight(EFonts::ESP_NAME) - 1,
						color,
						TXT_CENTERX,
						"Infected");
					std::string meter = "[" + std::to_string(distance) + "M]";
					G::Draw.String(EFonts::METER_THING, x + (w / 2),
						y + h + 2, // 2-pixel offset below the box
						{ 255,255,255,255 },
						TXT_CENTERX,
						meter.c_str());
				}
				
				break;
			}
			/* GROUP_SPECIAL, GROUP_BOSS, GROUP_PLAYER can only get helth. */
			case Boomer:
			case Smoker:
			case Hunter:
			case Jockey:
			case Spitter:
			case Charger:
			case Tank:
			{
				C_BaseEntity* pZombies = pEntity->As<C_BaseEntity*>();
				if (!GetBounds(pZombies, x, y, w, h) || !pZombies)
					break;
				if (!G::Util.IsInfectedAlive(pZombies->m_usSolidFlags(), pZombies->As<C_BaseAnimating*>()->m_nSequence(), pCC->m_ClassID))
				{
					continue;
				}
				const Color color = Vars::ESP::InfectedColor;

				const int nDrawX = x + (w / 2);
				int nDrawY = y + (h / 2);
				//int distance = (pLocal->GetAbsOrigin() - pZombies->GetAbsOrigin()).Lenght();
				int distance = U::Math.GetDistanceMeters(pLocal->GetAbsOrigin(), pZombies->GetAbsOrigin());
				if (Vars::ESP::Box) {
					G::Draw.OutlinedRect(x - 1, y - 1, w + 2, h + 2, Color(0, 0, 0, 255));
					G::Draw.OutlinedRect(x, y, w, h, color);
					G::Draw.OutlinedRect(x + 1, y + 1, w - 2, h - 2, Color(0, 0, 0, 255));
				}
				if (Vars::ESP::Name) {
					G::Draw.String(EFonts::ESP_NAME, x + (w / 2),
						y - G::Draw.GetFontHeight(EFonts::ESP_NAME) - 1,
						color,
						TXT_CENTERX,
						GetZombieName(pCC->m_ClassID)
					);
					std::string meter = "[" + std::to_string(distance) + "M]";
					G::Draw.String(EFonts::METER_THING, x + (w / 2),
						y + h + 2, // 2-pixel offset below the box
						{ 255,255,255,255 },
						TXT_CENTERX,
						meter.c_str());
				}
				const int nHealth = pZombies->GetHealth();
				const int nMaxHealth = pZombies->GetMaxHealth();
				const int newHealth = std::clamp(nHealth, 0, nMaxHealth);
				const Color clrHealth = G::Util.GetHealthColor(nHealth, nMaxHealth);
				if (Vars::ESP::Healthbar) 
				{
					float flHealth = static_cast<float>(newHealth);
					float flMaxHealth = static_cast<float>(nMaxHealth);
					int nHeight = (h + (flHealth < flMaxHealth ? 2 : 1));
					int nHeight2 = (h + 2);
					float ratio = (flHealth / flMaxHealth);
					G::Draw.Rect(x - 6, y - 1, 4, h + 2, Color(0, 0, 0, 255));
					G::Draw.Rect(x - 5, (y + nHeight - (nHeight * ratio)), 2, (nHeight * ratio) - 1, Color(0, 255, 0, 255));
				}
				break;
			}
			case Witch:
			{
				C_Witch* pWitches = pEntity->As<C_Witch*>();
				if (!GetBounds(pWitches, x, y, w, h) || !pWitches)
					break;
				if (!G::Util.IsInfectedAlive(pWitches->m_usSolidFlags(), pWitches->m_nSequence(), pCC->m_ClassID))
				{
					continue;
				}
				const Color color = Vars::ESP::InfectedColor;
				int distance = U::Math.GetDistanceMeters(pLocal->GetAbsOrigin(), pWitches->GetAbsOrigin());
				//(pLocal->GetAbsOrigin() - pWitches->GetAbsOrigin()).Lenght();
				const int nDrawX = x + (w / 2);
				int nDrawY = y + (h / 2);
				if (Vars::ESP::Box) {
					G::Draw.OutlinedRect(x - 1, y - 1, w + 2, h + 2, Color(0, 0, 0, 255));
					G::Draw.OutlinedRect(x, y, w, h, color);
					G::Draw.OutlinedRect(x + 1, y + 1, w - 2, h - 2, Color(0, 0, 0, 255));
				}
				if (Vars::ESP::Name) {
					G::Draw.String(EFonts::ESP_NAME, x + (w / 2),
						y - G::Draw.GetFontHeight(EFonts::ESP_NAME) - 1,
						color,
						TXT_CENTERX,
						GetZombieName(pCC->m_ClassID)
					);
					std::string meter = "[" + std::to_string(distance) + "M]";
					G::Draw.String(EFonts::METER_THING, x + (w / 2),
						y + h + 2, // 2-pixel offset below the box
						{ 255,255,255,255 },
						TXT_CENTERX,
						meter.c_str());
				}
				const int nHealth = pWitches->GetHealth();
				const int nMaxHealth = pWitches->GetMaxHealth();
				const Color clrHealth = G::Util.GetHealthColor(nHealth, nMaxHealth);
				
				/*
				gDraw.DrawString(x + w + 2, y + iY, clrPlayerCol, "%s", pEntity->szGetClass());
			iY += gDraw.GetESPHeight();
				*/
				break;
			}
			default:
				break;
		}
	}
}

bool CFeatures_ESP::GetBounds(C_BaseEntity* pBaseEntity, int& x, int& y, int& w, int& h)
{
	Vector vPoints[8];
	U::Math.BuildTransformedBox(vPoints, pBaseEntity->m_vecMins(), pBaseEntity->m_vecMaxs(), pBaseEntity->RenderableToWorldTransform());

	Vector flb, brt, blb, frt, frb, brb, blt, flt;
	if (G::Util.W2S(vPoints[3], flb) && G::Util.W2S(vPoints[5], brt)
		&& G::Util.W2S(vPoints[0], blb) && G::Util.W2S(vPoints[4], frt)
		&& G::Util.W2S(vPoints[2], frb) && G::Util.W2S(vPoints[1], brb)
		&& G::Util.W2S(vPoints[6], blt) && G::Util.W2S(vPoints[7], flt)
		&& G::Util.W2S(vPoints[6], blt) && G::Util.W2S(vPoints[7], flt))
	{
		const Vector vTransformed[8] = { flb, brt, blb, frt, frb, brb, blt, flt };

		float left = flb.x;
		float top = flb.y;
		float righ = flb.x;
		float bottom = flb.y;

		for (int n = 1; n < 8; n++)
		{
			if (left > vTransformed[n].x)
				left = vTransformed[n].x;

			if (top < vTransformed[n].y)
				top = vTransformed[n].y;

			if (righ < vTransformed[n].x)
				righ = vTransformed[n].x;

			if (bottom > vTransformed[n].y)
				bottom = vTransformed[n].y;
		}

		x = static_cast<int>(left);
		y = static_cast<int>(bottom);
		w = static_cast<int>(righ - left);
		h = static_cast<int>(top - bottom);

		return !(x > G::Draw.m_nScreenW || (x + w) < 0 || y > G::Draw.m_nScreenH || (y + h) < 0);
	}

	return false;
}