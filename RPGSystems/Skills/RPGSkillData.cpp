#include "pch.h"

#include "RPGSkillData.h"

#include "AimModule/RPGSkillAimModules.h"
#include "Core/GameInstance.h"
#include "DirectoryData.h"
#include "EffectModule/RPGSkillEffectModules.h"
#include "RPGSkillSystem.h"

RPGSkillData::~RPGSkillData()
{
	delete pAimModule;
	delete pEffectModule;
}

std::string RPGSkillData::GetDragAndDropIconFile(DirectoryData& directoryData) const
{
	if (RPGSkillSystem* pSkillSystem = Game::GetGameInstance().FindGameSystemSlow<RPGSkillSystem>())
	{
		return DirectoryData::ConcatenateSVGFilePathChecked(directoryData.sharedUI, pSkillSystem->GetSkillIconFileName());
	}
	return "";
}
