#pragma once

class EditorAssetDataComposite;

// Remove an entry from an EditorAssetDataComposite
class EditorActionRemoveCompositeData : public EditorActionBase
{
public:

	EditorActionRemoveCompositeData(std::weak_ptr<EditorAssetDataComposite> inDataComposite, EditorTypePropertyBase* inProperty, int inInsertIndex = -1)
	: pDataCompositeAsset(inDataComposite), pProperty(inProperty), insertIndex(inInsertIndex) {}

	~EditorActionRemoveCompositeData() override;

protected:
	//~ Begin EditorActionBase Interface
	void Undo() override;
	bool TryExecuteAction() override;
	std::string GetDescription() const override;
	//~ End EditorActionBase Interface


	std::weak_ptr<EditorAssetDataComposite> pDataCompositeAsset; // Used to check asset isn't deleted
	EditorTypePropertyBase* pProperty = nullptr;
	int insertIndex = -1;
	
};
