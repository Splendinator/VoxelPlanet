#include "pch.h"

#include "MenuScreenClassSelect.h"

#include "DirectoryData.h"
#include "ECS.h"

#include "UI/UIObjects/UIObjectButton.h"
#include "UI/UIObjects/UIObjectIconLoader.h"

void MenuScreenClassSelect::Init(UICanvas* pCanvas)
{
	DOMLOG_ERROR_IF(pRpgClassSystem == nullptr, "Need RPG class system")
	DOMLOG_ERROR_IF(pEcs == nullptr, "Need Ecs system")
	
	if (pDirectoryData)
	{
		onClickedDelegate.Bind(this, &MenuScreenClassSelect::OnClicked);
		
		for (MenuScreenClassSelectEntry& entry : classEntries)
		{
			entry.iconLoader = pCanvas->AddExistingUIObject<UIObjectIconLoader>(entry.loaderLayer);
			entry.iconLoader->LoadUIIcon(DirectoryData::ConcatenateFilePathChecked(pDirectoryData->sharedUI, classIconFileName, ".svg"), entry.classIconLayer);

			entry.button = pCanvas->AddExistingUIObject<UIObjectButton>(entry.loaderLayer); // Load button after icon so icon is clickable
			entry.button->onClicked.Add(onClickedDelegate);
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
		if (entry.button == params.pButton)
		{
			if (pRpgClassSystem && pEcs)
			{
				pRpgClassSystem->SetEntityClass(pEcs->GetPlayerEntityId(), entry.rpgClass, /*bSetMesh=*/true);
			}

			RequestCloseSelf();
			return;
		}
	}
}
