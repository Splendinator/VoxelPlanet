#pragma once

#include "EditorActionBase.h"

class EditorTypeClass;
class EditorAssetBase;
class EditorTypePropertyBase;

struct OnPropertyChangedData
{
	EditorTypePropertyBase* pProperty = nullptr;
	std::string oldValue;
	std::string newValue;
	std::shared_ptr<EditorTypeClass> oldClassValue;
	std::shared_ptr<EditorTypeClass> newClassValue;
};

struct ForceSetValueParams
{
	std::string newValue; // Most classes exclusively use this to set their value
	std::shared_ptr<EditorTypeClass> newClassValue; // EditInlineNew class properties also use this to set their EditInlineNew class
	// #NOTE: If we add any more params here, maybe add ForceSetValueParams to OnPropertyChangedData instead of oldValue newValue oldClassValue newClassValue
};

/** EditorActionChangeProperty
*
* This class is used for changing a property value of an EditorAssetBase.
*/
class EditorActionChangeProperty : public EditorActionBase
{
public:
	EditorActionChangeProperty(std::weak_ptr<EditorAssetBase> pInAsset, OnPropertyChangedData inPropertyChangeData,  const std::filesystem::path& inAssetPath) :
		pAsset(pInAsset), propertyChangeData(inPropertyChangeData), assetPath(inAssetPath) {}

	//~ Begin EditorActionBase Interface
	virtual void Undo() override;
	virtual bool TryExecuteAction() override;
	virtual std::string GetDescription() const override;
	//~ End EditorActionBase Interface

private:
	
	bool SetValue(const ForceSetValueParams& params);
	
	std::weak_ptr<EditorAssetBase> pAsset;
	OnPropertyChangedData propertyChangeData;
	std::filesystem::path assetPath;
};