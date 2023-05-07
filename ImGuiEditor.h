#pragma once

class CodeParseTokenBase;
class EditorActionBase;
class EditorAssetBase;
class EditorTypeBase;
class EditorWindowBase;

/** ImGuiEditor
*
* This class is used to edit the game in using ImGui. 
*
* It reads in all the code files in the project and parses them for classes, structs, enums, etc and lets you edit them.
*/
class ImGuiEditor
{
public:
	ImGuiEditor() {};

	void Init();
	void Uninit();
	void Tick();
	
	void AddWindow(const std::shared_ptr<EditorWindowBase>& pWindow);
	void RemoveWindow(EditorWindowBase* pWindow);

	void DoAction(const std::shared_ptr<EditorActionBase>& pAction);
	void Undo(); // Undo the last action
	void Redo(); // Redo the last undone action if possible

	// Add a new asset, className is the name of the class that this type will be (see templateTypes), assetName is the unique name of the asset to create
	void AddAsset(EditorAssetBase* pAsset);
	void RemoveAsset(EditorAssetBase* pAsset);
	
	// Get the type of a specified class, should be the same as the C++ class name
	EditorTypeBase* FindType(const std::string& typeName);
	std::vector<std::string> GetAllTypes();

private:
	
	// Generate template types. see templateTypes
	void CreateTemplateTypes(const std::string& typesFile);

	// This map will contain a single instance of each type (the key being it's name) and all of the EditorTypes will have default values.
	// i.e "SpellFireball" -> "EditorTypeClass(SpellFireball) with all the default numbers"
	std::unordered_map<std::string, EditorTypeBase*> templateTypes;

	// This map will contain names to their assets. so it might be "Fireball" to a fireball asset, etc.
	std::unordered_map<std::string, EditorAssetBase*> assets;
	
	// A list of windows currently showing.
	std::vector<std::shared_ptr<EditorWindowBase>> shownWindows;

	// List of all recently executed actions. Used for undo/redo
	std::vector<std::shared_ptr<EditorActionBase>> executedActions;
	int executedActionsIndex = -1; // The index of the last executed action. -1 means no actions have been executed yet.

	// Whether the editor is showing overlayed above the game -- if this is false don't do anything
	bool bEditorShowing = true;
};
