#pragma once
#include "../../SDK/SDK.h"
#include "../Vars.h"


class CMisc {
public:
	void run(C_TerrorPlayer* pLocal, CUserCmd* pCmd);
private:
	void movement(C_TerrorPlayer* pLocal, CUserCmd* pCmd);
};

namespace f { inline CMisc* misc; }