#include "GameUtil.h"

void CGlobal_GameUtil::FixMovement(const Vector vAngle, CUserCmd* cmd)
{
	Vector vMove = { cmd->forwardmove, cmd->sidemove, cmd->upmove }, vMoveAng;
	U::Math.VectorAngles(vMove, vMoveAng);

	const float flSpeed = ::sqrtf(vMove.x * vMove.x + vMove.y * vMove.y);
	const float flYaw = DEG2RAD(vAngle.y - cmd->viewangles.y + vMoveAng.y);

	cmd->forwardmove = (::cosf(flYaw) * flSpeed);
	cmd->sidemove = (::sinf(flYaw) * flSpeed);
}

void CGlobal_GameUtil::Trace(const Vector& start, const Vector& end, unsigned int mask, ITraceFilter* filter, trace_t* trace)
{
	Ray_t ray = { start, end };
	I::EngineTrace->TraceRay(ray, mask, filter, trace);
}

bool CGlobal_GameUtil::W2S(const Vector vWorld, Vector& vScreen)
{
	return !(I::DebugOverlay->ScreenPosition(vWorld, vScreen));
}

bool CGlobal_GameUtil::IsOnScreen(const Vector vWorld)
{
	Vector vScreen;
	return W2S(vWorld, vScreen);
}

bool CGlobal_GameUtil::IsValidTeam(const int nTeam)
{
	return ((nTeam == TEAM_SURVIVOR) || (nTeam == TEAM_INFECTED));
}

bool CGlobal_GameUtil::IsInfectedAlive(const int nSolidFlags, const int nSequence, int classindex)
{
	
	// works fine on infected.
	if ((nSolidFlags & FSOLID_NOT_SOLID) || (nSequence >= 305))
		return false;

	//These are from l4d1 and do not work with the mudfuckers on l4d2 at least.
	return !(U::Math.CompareGroup(nSequence, 303, 279, 295, 266, 302, 301, 281, 283, 261, 293,
		294, 297, 278, 277, 300, 299, 282, 276, 304, 292, 272, 396, 259, 260,
		271, 257, 280, 275, 285, 267, 258, 268, 273));
}
bool CGlobal_GameUtil::IsInvalid(C_TerrorPlayer* pLocal, IClientEntity* target) {
	if (!target) return false;
	int hit = HITGROUP_HEAD;
	ClientClass* pCC = target->GetClientClass();
	if (!pCC)
		return true;
	switch (pCC->m_ClassID)
	{
	case EClientClass::Witch:
	{
		C_Witch* pWitch = target->As<C_Witch*>();
		if (!pWitch) return true;
		if (!G::Util.IsInfectedAlive(pWitch->m_usSolidFlags(), pWitch->m_nSequence(), pCC->m_ClassID))
		{
			return true;
		}
		break;
	}
	case EClientClass::Tank:
	{
		hit = HITGROUP_STOMACH;
		C_Tank* pTank = target->As<C_Tank*>();
		if (!pTank) return true;
		if (pTank->deadflag()) return true;
		break;
	}
	case EClientClass::Boomer:
	case EClientClass::Jockey:
	case EClientClass::Smoker:
	case EClientClass::Hunter:
	case EClientClass::Spitter:
	case EClientClass::Charger:
	{
		if (pCC->m_ClassID == EClientClass::Boomer) { hit = HITGROUP_STOMACH; };
		C_TerrorPlayer* pPlayer = target->As<C_TerrorPlayer*>();
		if (!pPlayer) return true;
		if (pPlayer->deadflag()) return true;
		break;
	}
	case EClientClass::Infected:
	{
		hit = HITGROUP_STOMACH;
		C_Infected* pInfected = target->As<C_Infected*>();
		if (!pInfected) return true;
		if (!G::Util.IsInfectedAlive(pInfected->m_usSolidFlags(), pInfected->m_nSequence(), pCC->m_ClassID) || pInfected->m_bIsBurning())
		{
			return true;
		}
		break;
	}
	default:
		break;
	}
	/*
	C_BaseAnimating* pAnimating = target->As<C_BaseAnimating*>();
	Vector entityposition;
	if (!pAnimating->GetHitboxPositionByGroup(hit, entityposition))
	{
		return true;
	}
	*/
	return false;
}
Color CGlobal_GameUtil::GetHealthColor(const int nHealth, const int nMaxHealth)
{
	if (nHealth > nMaxHealth)
		return { 44u, 130u, 201u, 255u };

	const int nCurHP = U::Math.Max(0, U::Math.Min(nHealth, nMaxHealth));

	return {
		U::Math.Min((510 * (nMaxHealth - nCurHP)) / nMaxHealth, 200),
		U::Math.Min((510 * nCurHP) / nMaxHealth, 200),
		0u,
		255u
	};
}

IMaterial* CGlobal_GameUtil::CreateMaterial(const char* const szVars)
{
	static int nCreated = 0;

	char szOut[DT_MAX_STRING_BUFFERSIZE];
	sprintf_s(szOut, sizeof(szOut), _("pol_mat_%i.vmt"), nCreated++);

	char szMat[DT_MAX_STRING_BUFFERSIZE];
	sprintf_s(szMat, sizeof(szMat), szVars);

	KeyValues* pKvals = new KeyValues;

	G::KeyVals.Init(pKvals, (char*)szOut);
	G::KeyVals.LoadFromBuffer(pKvals, szOut, szMat);

	IMaterial* pMat = I::MaterialSystem->CreateMaterial(szOut, pKvals);

	if (!IsErrorMaterial(pMat))
		pMat->AddRef();

	return pMat;
}