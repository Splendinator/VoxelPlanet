#pragma once

#include "EditorActionBase.h"

// Base action for creating an asset. We need one of these per asset type (EditorAssetBase)
class EditorActionCreateAssetBase : public EditorActionBase
{
public:

	EditorActionCreateAssetBase(const std::filesystem::path& inTargetPath, const std::string& inAssetName)
	: targetPath(inTargetPath), assetName(inAssetName) {}

	//~ Begin EditorActionBase Interface
	virtual void Undo() override;
	virtual bool TryExecuteAction() override;
	virtual std::string GetDescription() const override;
	//~ End EditorActionBase Interface
	
protected:

	// Override this to create the specific asset type.
	virtual std::shared_ptr<EditorAssetBase> CreateAsset(const std::filesystem::path& assetFilePath) = 0; 

	std::filesystem::path targetPath;
	std::string assetName;

	// Pointer to the created asset. Only valid if the action was executed.
	std::weak_ptr<EditorAssetBase> pCreatedAsset;
	
};
