#include "pch.h"

#include "MenuScreenClassSelect.h"

#include "DirectoryData.h"
#include "ECS/ECS.h"
#include "RPGSystems/RPGSystem.h"
#include "UI/UIObjects/UIObjectButtonWithIcon.h"

void MenuScreenClassSelect::Init(UICanvas* pCanvas)
{
	DOMLOG_ERROR_IF(pRpgSystem == nullptr, "Need RPG system")
	DOMLOG_ERROR_IF(pEcs == nullptr, "Need Ecs system")
	
	if (pDirectoryData)
	{
		onClickedDelegate.Bind(this, &MenuScreenClassSelect::OnClicked);
		
		for (MenuScreenClassSelectEntry& entry : classEntries)
		{
			entry.buttonWithIcon = pCanvas->AddExistingUIObject<UIObjectButtonWithIcon>(entry.loaderLayer);
			entry.buttonWithIcon->GetButton().onClicked.Add(onClickedDelegate);
			entry.buttonWithIcon->LoadUIIcon(DirectoryData::ConcatenateSVGFilePathChecked(pDirectoryData->sharedUI, classIconFileName), entry.classIconLayer);
		}
	}
}

void MenuScreenClassSelect::UnInit()
{
	
}

void MenuScreenClassSelect::OnClicked(UIObjectButtonDelegateParams params)
{
	DOMASSERT(params.pButton)

	for (MenuScreenClassSelectEntry& entry : classEntries)
	{
		if (&entry.buttonWithIcon->GetButton() == params.pButton)
		{
			if (pRpgSystem && pEcs)
			{
				RPGEntitySetupParams entitySetupParams = {};
				entitySetupParams.pClassData = entry.pRpgClass;
				entitySetupParams.bUseClassMeshOverRaceMesh = true; // Player's mesh will change based off class choice
				entitySetupParams.startLevel = 0;
				entitySetupParams.pRaceData = pPlayerRace;
				pRpgSystem->SetupRPGEntity(pEcs->GetPlayerEntityId(), entitySetupParams);
			}

			RequestCloseSelf();
			return;
		}
	}
}
