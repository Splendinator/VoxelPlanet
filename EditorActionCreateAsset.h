#include "EditorActionBase.h"
#include "ImGuiEditor.h"
#include <string>

class EditorAssetClass;

/** EditorActionCreateAsset
*
* This class is used for creating assets in the editor.
*/
class EditorActionCreateAsset : public EditorActionBase
{
public:
	EditorActionCreateAsset(const std::filesystem::path& inTargetPath, const std::string& inClassName, const std::string& inAssetName, ImGuiEditor& inEditor) : 
		targetPath(inTargetPath), className(inClassName), assetName(inAssetName), editor(inEditor) {}

	//~ Begin EditorActionBase Interface
	virtual void Undo() override;
	virtual bool TryExecuteAction() override;
	virtual std::string GetDescription() const override;
	//~ End EditorActionBase Interface

private:
	std::filesystem::path targetPath;
	std::string className;
	std::string assetName;
	ImGuiEditor& editor;

	// Pointer to the created asset. Only valid if the action was executed.
	std::weak_ptr<EditorAssetBase> pCreatedAsset;
};