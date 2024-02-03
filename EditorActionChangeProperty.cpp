#include "pch.h"

#include "EditorActionChangeProperty.h"

#include "EditorAssetBase.h"
#include "EditorTypePropertyBase.h"

// #TODO: if pAsset expires then this doesn't work any more. Need a way to delete actions if the asset is deleted

void EditorActionChangeProperty::Undo()
{
	if (pAsset.expired())
	{
		return;
	}
	SetValue(previousValue);
}

bool EditorActionChangeProperty::TryExecuteAction()
{
	if (pAsset.expired())
	{
		return false;
	}
	return SetValue(nextValue);
}

std::string EditorActionChangeProperty::GetDescription() const
{
	const std::string& name = pAsset.expired() ? "UNKNOWN" : pProperty->GetName();
	return "Change Property " + name + " from " + previousValue + " to " + nextValue;
}

bool EditorActionChangeProperty::SetValue(const std::string& value)
{
	pProperty->ForceSetValue(value);

	std::ofstream outFile(assetPath);
	if (!outFile.is_open())
	{
		DOMLOG_ERROR("Cannot open asset path", assetPath.string());
		return false;
	}

	pAsset.lock()->WriteToFile(outFile);

	return true;
}
