#pragma once

#include "EditorActionBase.h"

class EditorAssetBase;
class EditorTypePropertyBase;

/** EditorActionChangeProperty
*
* This class is used for changing a property value of an EditorAssetBase.
*/
class EditorActionChangeProperty : public EditorActionBase
{
public:
	EditorActionChangeProperty(EditorTypePropertyBase* pInProperty, std::weak_ptr<EditorAssetBase> pInAsset, const std::string& inPreviousValue, const std::string& inNextValue, const std::filesystem::path& inAssetPath) :
		pProperty(pInProperty), pAsset(pInAsset), previousValue(inPreviousValue), nextValue(inNextValue), assetPath(inAssetPath) {}

	//~ Begin EditorActionBase Interface
	virtual void Undo() override;
	virtual bool TryExecuteAction() override;
	virtual std::string GetDescription() const override;
	//~ End EditorActionBase Interface

private:
	
	bool SetValue(const std::string& value);

	EditorTypePropertyBase* pProperty = nullptr;
	std::weak_ptr<EditorAssetBase> pAsset;
	std::string previousValue;
	std::string nextValue;
	std::filesystem::path assetPath;
};