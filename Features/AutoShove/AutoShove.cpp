#include "AutoShove.h"
#include "../Vars.h"

bool CanShoveWitch(int rage) {
	if (rage == 1)
		return true;
	else
		return false;
	return false;

}
bool CAutoShove::CanShove(C_TerrorPlayer* pLocal, C_Infected* pInfected) {
	if (!pLocal || !pInfected)
		return false;

	if (pInfected->IsDormant())
		return false;
	if (!pInfected->ValidEntity(pInfected->m_nSequence(), pInfected->m_usSolidFlags()))
		return false;
	if (!pLocal->GetActiveWeapon())
		return false;
	if (!pLocal->CanBeShoved() || !pLocal->IsReadyToShove())
		return false;
	if (pInfected->GetClientClass()->m_ClassID == Witch && Vars::Ignore::IgnoreWitchUntilStartled
		&& !CanShoveWitch(pInfected->As<C_Witch*>()->m_rage()))
		return false;

	int distance = (pLocal->GetAbsOrigin() - pInfected->GetAbsOrigin()).Lenght();
	if (distance > 87) // if they are very close, then do it.
		return false;

	return true;
}
#include "../../SDK/EntityCache/entitycache.h"
C_BaseEntity* CAutoShove::GetBestTarget(C_TerrorPlayer* pLocal) {
	C_BaseEntity* BestEntity = nullptr;
	float FOV = FLT_MAX;

	for (const auto pInfected : gEntityCache.GetGroup(EGroupType::INFECTED))
	{
		if (!pInfected)
			continue;
		if (!CanShove(pLocal, pInfected->As<C_Infected*>()))
			continue;
		BestEntity = pInfected->As<C_Infected*>();
	}
	return BestEntity;
}


bool PosTwo(C_BaseEntity* pSkip, C_BaseEntity* pEntity, Vector from, Vector to)
{
	Ray_t ray;
	ray.Init(from, to);

	const uint32_t mask = (MASK_SHOT);

	CTraceFilter filter;
	filter.pSkip = pSkip;

	trace_t trace;
	I::EngineTrace->TraceRay(ray, mask, &filter, &trace);

	return ((trace.m_pEnt && trace.m_pEnt == pEntity));
}


Vector GetBestHitbox(C_BaseEntity* pEntity, C_TerrorPlayer* pLocal)
{
	Vector vTarget;
	C_BaseAnimating* pAnimating = pEntity->As<C_BaseAnimating*>();
	if (!pAnimating)
		return Vector();
	if (!pAnimating->GetHitboxPositionByGroup(HITGROUP_HEAD, vTarget))
		return Vector();
	if (!PosTwo(pLocal, pEntity, pLocal->EyePosition(), vTarget))
		return Vector(0, 0, 0);

	return vTarget;
}

Vector GetBestAngle(C_BaseEntity* pEntity, C_TerrorPlayer* pLocal)
{
	auto Hitbox = GetBestHitbox(pEntity, pLocal);
	if (Hitbox.IsZero())
		return Vector();

	return U::Math.CalcAngle(pLocal->EyePosition(), GetBestHitbox(pEntity, pLocal));
}

inline void FixMovementAgain(CUserCmd* pCmd, Vector m_vOldAngles, float m_fOldForward, float m_fOldSidemove)
{
	float deltaView = pCmd->viewangles.y - m_vOldAngles.y;
	float f1;
	float f2;

	if (m_vOldAngles.y < 0.f)
		f1 = 360.0f + m_vOldAngles.y;
	else
		f1 = m_vOldAngles.y;

	if (pCmd->viewangles.y < 0.0f)
		f2 = 360.0f + pCmd->viewangles.y;
	else
		f2 = pCmd->viewangles.y;

	if (f2 < f1)
		deltaView = abs(f2 - f1);
	else
		deltaView = 360.0f - abs(f1 - f2);
	deltaView = 360.0f - deltaView;

	pCmd->forwardmove = cos(DEG2RAD(deltaView)) * m_fOldForward + cos(DEG2RAD(deltaView + 90.f)) * m_fOldSidemove;
	pCmd->sidemove = sin(DEG2RAD(deltaView)) * m_fOldForward + sin(DEG2RAD(deltaView + 90.f)) * m_fOldSidemove;
}


void CAutoShove::run(C_TerrorPlayer* pLocal, CUserCmd* pCmd) {
	if (!Vars::Misc::AutoShove)
		return;
	if (!pLocal || !pCmd)
		return;

	auto pEntity = GetBestTarget(pLocal);
	if (!pEntity)
		return;
	if (pLocal->m_iTeamNum() != TEAM_SURVIVOR)
		return; // if we're infected, dont fucking do hitscan aimbot..
	if (pLocal->m_isGhost())
		return;
	if (!pLocal->GetActiveWeapon())
		return;
	if (!PosTwo(pLocal, pEntity, pLocal->EyePosition(), GetBestHitbox(pEntity, pLocal)))
		return; // Visibility check

	Vector m_vOldViewAngle = pCmd->viewangles;
	float m_fOldSideMove = pCmd->sidemove;
	float m_fOldForwardMove = pCmd->forwardmove;

	Vector Angle = GetBestAngle(pEntity, pLocal);

	if (Angle.IsZero())
		return;

	pCmd->viewangles = Angle;
	FixMovementAgain(pCmd, m_vOldViewAngle, m_fOldForwardMove, m_fOldSideMove);
	pCmd->buttons |= IN_ATTACK2;
}
