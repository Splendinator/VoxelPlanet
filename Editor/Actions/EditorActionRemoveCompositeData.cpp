#include "pch.h"

#include "EditorActionRemoveCompositeData.h"

#include "Editor/Assets/EditorAssetDataComposite.h"

EditorActionRemoveCompositeData::~EditorActionRemoveCompositeData()
{
	if (pProperty && state == EEditorActionState::Done)
	{
		// If the action is undone, the property is not in the asset so this is the sole owner of it.
		delete pProperty;
	}
}

void EditorActionRemoveCompositeData::Undo()
{
	if (pDataCompositeAsset.expired() || !pDataCompositeAsset.lock()->IsInAssetManager())
	{
		DOMLOG_ERROR("I don't think this should be hit unless the asset is deleted unexpectedly");
		return;
	}

	pDataCompositeAsset.lock()->GetEditorTypeDataComposite().AddAndSetupProperty(pProperty, insertIndex);
	pDataCompositeAsset.lock()->SaveAsset();
}

bool EditorActionRemoveCompositeData::TryExecuteAction()
{
	if (pDataCompositeAsset.expired() || !pDataCompositeAsset.lock()->IsInAssetManager())
	{
		DOMLOG_ERROR("I don't think this should be hit unless the asset is deleted unexpectedly");
		return false;
	}
	
	pDataCompositeAsset.lock()->GetEditorTypeDataComposite().RemoveProperty(pProperty, false);
	pDataCompositeAsset.lock()->SaveAsset();
	return true;
}

std::string EditorActionRemoveCompositeData::GetDescription() const
{
	return "Remove DataComposite Property: " + pProperty->GetName();
}

