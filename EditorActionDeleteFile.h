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
	EditorActionDeleteFile(const std::filesystem::path& inTargetAsset) : targetFile(inTargetAsset) {}

	//~ Begin EditorActionBase Interface
	void Undo() override;
	bool TryExecuteAction() override;
	std::string GetDescription() const override;
	//~ End EditorActionBase Interface

private:
	
	std::filesystem::path targetFile;

	dmut::HeapAllocSize<char> deletedFileContents; // used to Undo()
};