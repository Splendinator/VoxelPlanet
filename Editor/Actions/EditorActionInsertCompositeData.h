#pragma once

class EditorAssetDataComposite;

// Inserts an entry into an EditorAssetDataComposite
class EditorActionInsertCompositeData : public EditorActionBase
{
public:

	EditorActionInsertCompositeData(std::weak_ptr<EditorAssetDataComposite> inDataComposite, EditorTypePropertyBase* inProperty, int inInsertIndex = -1)
	: pDataCompositeAsset(inDataComposite), pProperty(inProperty), insertIndex(inInsertIndex) {}

	~EditorActionInsertCompositeData() override;

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
