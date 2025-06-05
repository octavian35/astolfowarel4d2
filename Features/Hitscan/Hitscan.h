#pragma once
#include "../../SDK/SDK.h"


class chitscan {
private:
	Vector GetBestHitbox(C_BaseEntity* pEntity, C_TerrorPlayer* pLocal);
	Vector GetBestAngle(C_BaseEntity* pEntity, C_TerrorPlayer* pLocal);
	C_BaseEntity* GetBestTarget(C_TerrorPlayer* pLocal);
public:
	void run(C_TerrorPlayer* pLocal, CUserCmd* pCmd);
};
namespace f { inline chitscan hitscan; }