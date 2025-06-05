#pragma once
#include "../../SDK/SDK.h"

class CAutoShove {
public:
	void run(C_TerrorPlayer* pLocal, CUserCmd* pCmd);
private:
	bool CanShove(C_TerrorPlayer* pLocal, C_Infected* pInfected);
	C_BaseEntity* GetBestTarget(C_TerrorPlayer* pLocal);
};

namespace f { inline CAutoShove autoshove; }