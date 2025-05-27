#include "EditorActionBase.h"

#include <filesystem>

#include "DomUtils/HeapAlloc.h"

class EditorAssetClass;

/** EditorActionDeleteFile
*
* This class is used to delete a file
*/
class EditorActionDeleteFile : public EditorActionBase
{
public:
	EditorActionDeleteFile(const std::filesystem::path& inTargetAsset, std::string inAssetName) : targetFile(inTargetAsset), assetName(inAssetName) {}

	//~ Begin EditorActionBase Interface
	void Undo() override;
	bool TryExecuteAction() override;
	std::string GetDescription() const override;
	//~ End EditorActionBase Interface

private:
	
	std::filesystem::path targetFile;
	std::string assetName;

	dmut::HeapAllocSize<char> deletedFileContents; // used to Undo()

	std::shared_ptr<EditorAssetBase> pDeletedAsset; // After deleting we cache the asset here so we can undo deleting it
};