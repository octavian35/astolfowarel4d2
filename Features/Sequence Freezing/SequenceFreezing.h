#pragma once
#include "../../SDK/SDK.h"

class IDFDemo
{
private:
	void RemoveConds(C_TerrorPlayer* local, CUserCmd* cmd, int value, bool disableattack);
	void AllahuAkbar(CUserCmd* pCmd, C_TerrorPlayer* pLocal);
public:
	void Run(CUserCmd* pCmd, C_TerrorPlayer* pLocal);
};
namespace f { inline IDFDemo SequenceFreezing; }