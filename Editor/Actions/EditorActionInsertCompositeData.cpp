#include "pch.h"

#include "EditorActionInsertCompositeData.h"

#include "Editor/Assets/EditorAssetDataComposite.h"

EditorActionInsertCompositeData::~EditorActionInsertCompositeData()
{
	if (pProperty && state == EEditorActionState::NotDone)
	{
		// If the action is undone, the property is not in the asset so this is the sole owner of it.
		delete pProperty;
	}
	
}

void EditorActionInsertCompositeData::Undo()
{
	if (pDataCompositeAsset.expired() || !pDataCompositeAsset.lock()->IsInAssetManager())
	{
		DOMLOG_ERROR("I don't think this should even be hit unless the asset is deleted unexpectedly");
		return;
	}
	
	pDataCompositeAsset.lock()->GetEditorTypeDataComposite().RemoveProperty(pProperty);
	pDataCompositeAsset.lock()->SaveAsset();
}

bool EditorActionInsertCompositeData::TryExecuteAction()
{
	if (pDataCompositeAsset.expired() || !pDataCompositeAsset.lock()->IsInAssetManager())
	{
		DOMLOG_ERROR("I don't think this should even be hit unless the asset is deleted unexpectedly");
		return false;
	}

	pDataCompositeAsset.lock()->GetEditorTypeDataComposite().AddAndSetupProperty(pProperty, insertIndex);
	pDataCompositeAsset.lock()->SaveAsset();
	return true;
}

std::string EditorActionInsertCompositeData::GetDescription() const
{
	return "Create DataComposite Property: " + pProperty->GetName();
}
