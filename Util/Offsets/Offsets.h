#pragma once

#include "../Pattern/Pattern.h"

class CUtil_Offsets
{
public:
	void Init();

public:
	DWORD m_dwStartDrawing = 0x0;
	DWORD m_dwFinishDrawing = 0x0;
	DWORD m_dwGlobalVars = 0x0;
	DWORD m_dwMoveHelper = 0x0;
	DWORD m_dwClientMode = 0x0;
	DWORD m_dwSharedRandomFloat = 0x0;
	DWORD m_dwRandomSeed = 0x0;
	DWORD m_dwCheckForSequenceChange = 0x0;
	DWORD m_dwCLMove = 0x0;
	DWORD m_dwCalcPlayerView = 0x0;
	DWORD m_dwUpdateSpread = 0x0;
	DWORD m_dwDrawModels = 0x0;
	DWORD m_dwAvoidPlayers = 0x0;
	DWORD m_dwPhysicsRunThink = 0x0;
	DWORD m_dwSetPredictionRandomSeed = 0x0;
	DWORD m_dwGetSurvivorSet = 0x0;
	DWORD m_dwCLSendMove = 0x0;
	DWORD m_dwIInput = 0x0;
	DWORD m_dwSequenceName = 0x0;
	DWORD m_dwBaseEntityInterpolate = 0x0;
	DWORD m_dwGetClientInterpAmount = 0x0;
};

namespace U { inline CUtil_Offsets Offsets; }