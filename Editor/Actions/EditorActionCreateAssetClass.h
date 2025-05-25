#include "EditorActionCreateAssetBase.h"

#include <string>


class AssetManager;
class EditorAssetBase;
class EditorAssetClass;

/** EditorActionCreateAssetClass
*
* This action creates an EditorAssetClass.
*/
class EditorActionCreateAssetClass : public EditorActionCreateAssetBase
{
public:
	EditorActionCreateAssetClass(const std::filesystem::path& inTargetPath, const std::string& inClassName, const std::string& inAssetName) : EditorActionCreateAssetBase(inTargetPath, inAssetName), className(inClassName) {}

	//~ Begin EditorActionBase Interface
	virtual bool TryExecuteAction() override;
	//~ End EditorActionBase Interface

protected:
	//~ Begin EditorActionCreateAssetClass Interface
	std::shared_ptr<EditorAssetBase> CreateAsset(const std::filesystem::path& assetFilePath) override;
	//~ End EditorActionCreateAssetClass Interface
	
private:
	
	std::string className;
};