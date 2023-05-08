#include "pch.h"

#include "EditorActionChangeProperty.h"

#include "EditorAssetBase.h"
#include "EditorTypePropertyBase.h"

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
	// TODO: Implement GetDescription functionality
	return "Change Property " + pProperty->GetName() + " from " + previousValue + " to " + nextValue;
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
