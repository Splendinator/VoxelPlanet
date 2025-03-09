#include "pch.h"

#include "RPGSkillData.h"

#include "DirectoryData.h"
#include "RPGSkillSystem.h"

#include "Core/GameInstance.h"

std::string RPGSkillData::GetDragAndDropIconFile(DirectoryData& directoryData) const
{
	if (RPGSkillSystem* pSkillSystem = Game::GetGameInstance().FindGameSystemSlow<RPGSkillSystem>())
	{
		return DirectoryData::ConcatenateSVGFilePathChecked(directoryData.sharedUI, pSkillSystem->GetSkillIconFileName());
	}
	return "";
}
