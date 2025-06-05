#include "EnginePrediction.h"

bool DoSpreadPred(C_TerrorWeapon* pWeapon)
{
	if (!pWeapon)
		return false;
	if (pWeapon->GetCurrentSpread() == -1.0f)
		return false;
	if (pWeapon->GetCurrentSpread() == 0.0f)
		return false;
	if (pWeapon->GetWeaponID() == WEAPON_MELEE)
		return false;
	switch (pWeapon->GetWeaponID())
	{
	case WEAPON_AK47:
	case WEAPON_AWP:
	case WEAPON_DEAGLE:
	case WEAPON_HUNTING_RIFLE:
	case WEAPON_M16A1:
	case WEAPON_M60:
	case WEAPON_MAC10:
	case WEAPON_MILITARY_SNIPER:
	case WEAPON_MP5:
	case WEAPON_PISTOL:
	case WEAPON_SCAR:
	case WEAPON_SCOUT:
	case WEAPON_SSG552:
	case WEAPON_UZI:
		return true;
	default:
		break;
	}

	return false;
}

void CFeatures_EnginePrediction::Start(C_BasePlayer* pLocal, CUserCmd* cmd)
{
	if (!pLocal || !pLocal->GetActiveWeapon())
		return;
	m_flOldCurTime = I::GlobalVars->curtime;
	m_flOldFrameTime = I::GlobalVars->frametime;
	m_nOldTickCount = I::GlobalVars->tickcount;

	const int nOldTickBase = pLocal->m_nTickBase();
	const int nOldFired = pLocal->As<C_CSPlayer*>()->m_iShotsFired();
	const int nOldFlags = pLocal->m_fFlags();
	const int nTickBase = GetTickBase(nOldTickBase, cmd);
	float flOldSpread = 0;
	const Vector flOldBulletSpread;
	if (DoSpreadPred(pLocal->GetActiveWeapon()->As<C_TerrorWeapon*>()))
		flOldSpread = pLocal->GetActiveWeapon()->As<C_TerrorWeapon*>()->GetCurrentSpread();



	//StartCommand
	
	{
		cmd->random_seed = (MD5_PseudoRandom(cmd->command_number) & INT_MAX);
		reinterpret_cast<void(*)(CUserCmd*)>(U::Offsets.m_dwSetPredictionRandomSeed)(cmd);
	}
	

	I::GlobalVars->curtime = TICKS_TO_TIME(nTickBase);
	I::GlobalVars->frametime = TICK_INTERVAL;
	I::GlobalVars->tickcount = nTickBase;

	//pLocal->UpdateButtonState(cmd->buttons);
	I::Prediction->SetLocalViewAngles(cmd->viewangles);
	I::MoveHelper->SetHost(pLocal);

	I::Prediction->SetupMove(pLocal, cmd, I::MoveHelper, &m_MoveData);
	I::GameMovement->ProcessMovement(pLocal, &m_MoveData);
	I::Prediction->FinishMove(pLocal, cmd, &m_MoveData);

	//I::Prediction->RunCommand(pLocal, cmd, I::MoveHelper);
	I::MoveHelper->SetHost(nullptr);

	pLocal->m_nTickBase() = nOldTickBase;
	pLocal->As<C_CSPlayer*>()->m_iShotsFired() = nOldFired;
	pLocal->m_fFlags() = nOldFlags;
	// fix non-zero ping spread 
	// (prediction datamap spread fix.. not really specifically datamap fix but this still works..)
	if (DoSpreadPred(pLocal->GetActiveWeapon()->As<C_TerrorWeapon*>()))
	{
		pLocal->GetActiveWeapon()->As<C_TerrorWeapon*>()->UpdateSpread();
		pLocal->GetActiveWeapon()->As<C_TerrorWeapon*>()->GetCurrentSpread() = flOldSpread;
	}

}

void CFeatures_EnginePrediction::Finish(C_BasePlayer* pLocal, CUserCmd* cmd)
{
	//FinishCommand
	{
		reinterpret_cast<void(*)(CUserCmd*)>(U::Offsets.m_dwSetPredictionRandomSeed)(nullptr);
	}
	I::GlobalVars->curtime = m_flOldCurTime;
	I::GlobalVars->frametime = m_flOldFrameTime;
	I::GlobalVars->tickcount = m_nOldTickCount;
}



//CasualHacker I believe posted this.
int CFeatures_EnginePrediction::GetTickBase(const int nCurrent, CUserCmd* cmd)
{
	static int s_nTick = 0;
	static CUserCmd* s_pLastCommand = nullptr;

	if (cmd)
	{
		if (!s_pLastCommand || s_pLastCommand->hasbeenpredicted)
			s_nTick = nCurrent;
		else
			s_nTick++;

		s_pLastCommand = cmd;
	}

	return s_nTick;
}