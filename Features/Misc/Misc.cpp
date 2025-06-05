#include "Misc.h"
#include "../../SDK/L4D2/Interfaces/IConVar.h"
#include "../../SDK/EntityCache/entitycache.h"
#include <Windows.h>
#include <iostream>

void AutoStrafe(C_TerrorPlayer* pLocal, CUserCmd* pCmd)
{
	if (!Vars::Misc::AutoStrafe)
		return;
	//ConVar* noclip = I::Cvars->FindVar("noclip");
	if (pLocal) {
		static bool was_jumping = false;
		bool is_jumping = pCmd->buttons & IN_JUMP;

		if ((!is_jumping || was_jumping) && !(pLocal->m_fFlags() & FL_ONGROUND))
		{
			Vector Velocity = pLocal->m_vecVelocity();
			//pLocal->EstimateAbsVelocity(Velocity);
			const float speed = Velocity.Lenght2D();
			auto vel = Velocity;

			if (pLocal->m_MoveType() == MOVETYPE_NOCLIP
				|| pLocal->m_MoveType() == MOVETYPE_LADDER
				|| pLocal->m_MoveType() == MOVETYPE_OBSERVER)
			{
				return;
			}
			//if (pLocal->as<C_BasePlayer>()->m_water_level() > 1)
			//	return;
			//if (pLocal->GetFlags() & FL_SWIM) // idk if FL_SWIM works l0l!
			//	return;
			if (speed < 2.0f)
				return;

			constexpr auto perfectDelta = [](float speed, C_TerrorPlayer* pLocal) noexcept
			{
				static auto speedVar = pLocal->m_flMaxspeed();
				static auto airVar = 10.0f; //I::Cvars->FindVar("sv_airaccelerate");

				static auto wishSpeed = 15.0f;

				const auto term = wishSpeed / airVar / speedVar * 100.f / speed;

				if (term < 1.0f && term > -1.0f)
					return acosf(term);
				return 0.0f;
			};

			const float pDelta = perfectDelta(speed, pLocal);
			if (pDelta)
			{
				const float yaw = DEG2RAD(pCmd->viewangles.y);
				const float velDir = atan2f(vel.y, vel.x) - yaw;
				const float wishAng = atan2f(-pCmd->sidemove, pCmd->forwardmove);
				const float delta = U::Math.angleDiffRad(velDir, wishAng);

				float moveDir = delta < 0.0f ? velDir + pDelta : velDir - pDelta;

				pCmd->forwardmove = cosf(moveDir) * 450.f;
				pCmd->sidemove = -sinf(moveDir) * 450.f;
			}


		}
		was_jumping = is_jumping;
	}
}
void CMisc::movement(C_TerrorPlayer* pLocal, CUserCmd* pCmd)
{
	AutoStrafe(pLocal, pCmd);
	if (!(pLocal->m_fFlags() & FL_ONGROUND) && pCmd->buttons & IN_JUMP)
	{
		//Autostrafe	
		//if (Vars::Misc::AutoStrafe)
		//	if (pCmd->mousedx > 1 || pCmd->mousedx < -1)  //> 1 < -1 so we have some wiggle room
		//		pCmd->sidemove = pCmd->mousedx > 1 ? 450.f : -450.f;

		//Bunnyhop
		if (Vars::Misc::Bunnyhop)
			pCmd->buttons &= ~IN_JUMP;
	}
}


void CMisc::run(C_TerrorPlayer* pLocal, CUserCmd* pCmd) {
	if (!I::EngineClient->IsInGame())
		return;
	movement(pLocal, pCmd);
}