#include "ModelRender.h"
#include "../../Features/Vars.h"
using namespace Hooks;


void __fastcall ModelRender::ForcedMaterialOverride::Detour(void* ecx, void* edx, IMaterial* newMaterial, OverrideType_t nOverrideType)
{
	Table.Original<FN>(Index)(ecx, edx, newMaterial, nOverrideType);
}

void OverridematerialXQZ(IMaterial* mat, float r, float g, float b)
{
	mat->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ, true);
	mat->SetMaterialVarFlag(MATERIAL_VAR_ZNEARER, true);
	mat->SetMaterialVarFlag(MATERIAL_VAR_NOCULL, true);
	mat->SetMaterialVarFlag(MATERIAL_VAR_NOFOG, true);
	mat->SetMaterialVarFlag(MATERIAL_VAR_HALFLAMBERT, true);
	mat->ColorModulate(r / 255, g / 255, b / 255);
	I::ModelRender->ForcedMaterialOverride(mat);
}

void Overridematerial(IMaterial* mat, float r, float g, float b)
{
	mat->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ, false);
	mat->SetMaterialVarFlag(MATERIAL_VAR_ZNEARER, true);
	mat->SetMaterialVarFlag(MATERIAL_VAR_NOCULL, true);
	mat->SetMaterialVarFlag(MATERIAL_VAR_NOFOG, true);
	mat->SetMaterialVarFlag(MATERIAL_VAR_HALFLAMBERT, true);
	mat->ColorModulate(r / 255, g / 255, b / 255);
	I::ModelRender->ForcedMaterialOverride(mat);
}

void __fastcall ModelRender::DrawModelExecute::Detour(void* ecx, void* edx, const DrawModelState_t& state, const ModelRenderInfo_t& pInfo, matrix3x4_t* pCustomBoneToWorld)
{
	if (!I::EngineClient->IsInGame())
		Table.Original<FN>(Index)(ecx, edx, state, pInfo, pCustomBoneToWorld);
	static IMaterial* material = I::MaterialSystem->FindMaterial("debug/debugambientcube", "Model textures");
	static IMaterial* vomitboomer = I::MaterialSystem->FindMaterial(("particle/screenspaceboomervomit"), "Particle textures");

	if (pInfo.pModel && pInfo.entity_index && material)
	{
		if (vomitboomer)
		{
			vomitboomer->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW, Vars::Removals::BoomerVisual);
		}
		C_BaseEntity* pEntity = I::ClientEntityList->GetClientEntity(pInfo.entity_index)->As<C_BaseEntity*>();
		if (pEntity && !pEntity->IsDormant())
		{
			if (pEntity->GetClientClass())
			{
				if (Vars::Chams::Players)
				{
					if (pEntity->GetClientClass()->m_ClassID == SurvivorBot
						|| pEntity->GetClientClass()->m_ClassID == CTerrorPlayer)
					{
						const bool bIsSurvivor = (pEntity->As<C_TerrorPlayer*>()->GetTeamNumber() == TEAM_SURVIVOR);
						// if it is survivor       if not 
						const Color clrTeam = bIsSurvivor ? Vars::ESP::PlayerColor : Vars::ESP::PlayerInfectedColor;

						if (pEntity->As<C_TerrorPlayer*>()->IsAlive())
						{
							OverridematerialXQZ(material, clrTeam.r(), clrTeam.g(), clrTeam.b());
							Table.Original<FN>(Index)(ecx, edx, state, pInfo, pCustomBoneToWorld);
						}
						Overridematerial(material, clrTeam.r(), clrTeam.g(), clrTeam.b());
						/*
						if (pEntity->m_iTeamNum() == TEAM_INFECTED
							&& pEntity->ValidEntity(pEntity->As<C_TerrorPlayer*>()->m_nSequence(), pEntity->As<C_TerrorPlayer*>()->m_usSolidFlags()))
						{
							OverridematerialXQZ(material, Vars::Chams::PlayerInfectedColor.r(), Vars::Chams::PlayerInfectedColor.g(), Vars::Chams::PlayerInfectedColor.b());
							Table.Original<FN>(Index)(ecx, edx, state, pInfo, pCustomBoneToWorld);
						}
						*/
					}
				}
				if (Vars::Chams::Infected)
				{
					if (pEntity->IsZombie())
					{
						if (pEntity->ValidEntity(pEntity->As<C_Infected*>()->m_nSequence(), pEntity->As<C_Infected*>()->m_usSolidFlags()))
						{
							OverridematerialXQZ(material, Vars::Chams::InfectedColor.r(), Vars::Chams::InfectedColor.g(), Vars::Chams::InfectedColor.b());
							Table.Original<FN>(Index)(ecx, edx, state, pInfo, pCustomBoneToWorld);
						}
						Overridematerial(material, Vars::Chams::InfectedColor.r(), Vars::Chams::InfectedColor.g(), Vars::Chams::InfectedColor.b());
					}
				}
			}
		}
	}

	Table.Original<FN>(Index)(ecx, edx, state, pInfo, pCustomBoneToWorld);
	I::ModelRender->ForcedMaterialOverride(nullptr);
}

void ModelRender::Init()
{
	XASSERT(Table.Init(I::ModelRender) == false);
	XASSERT(Table.Hook(&ForcedMaterialOverride::Detour, ForcedMaterialOverride::Index) == false);
	XASSERT(Table.Hook(&DrawModelExecute::Detour, DrawModelExecute::Index) == false);
}