#include "pch.h"

#include "EditorActionChangeProperty.h"

#include "Editor/Assets/EditorAssetBase.h"
#include "Editor/Types/Properties/EditorTypePropertyBase.h"

// #TODO: if pAsset expires then this doesn't work any more. Need a way to delete actions if the asset is deleted

void EditorActionChangeProperty::Undo()
{
	if (pAsset.expired())
	{
		return;
	}
	SetValue({propertyChangeData.oldValue, propertyChangeData.oldClassValue});
}

bool EditorActionChangeProperty::TryExecuteAction()
{
	if (pAsset.expired())
	{
		return false;
	}
	return SetValue({propertyChangeData.newValue, propertyChangeData.newClassValue});
}

std::string EditorActionChangeProperty::GetDescription() const
{
	const std::string& name = pAsset.expired() ? "UNKNOWN" : propertyChangeData.pProperty->GetName();
	return "Change Property " + name + " from " + propertyChangeData.oldValue + " to " + propertyChangeData.newValue;
}

bool EditorActionChangeProperty::SetValue(const ForceSetValueParams& params)
{
	propertyChangeData.pProperty->ForceSetValue(params);

	std::ofstream outFile(assetPath);
	if (!outFile.is_open())
	{
		DOMLOG_ERROR("Cannot open asset path", assetPath.string());
		return false;
	}

	pAsset.lock()->WriteToFile(outFile);

	return true;	
}
