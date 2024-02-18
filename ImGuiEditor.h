#pragma once

class CodeParseTokenBase;
class EditorActionBase;
class EditorAssetBase;
class EditorTypeBase;
class EditorTypeClass;
class EditorTypeStruct;
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

	// Add a new asset, className is the name of the class that this type will be (see templateClassTypes), assetName is the unique name of the asset to create
	void AddAsset(std::shared_ptr<EditorAssetBase> pAsset);
	void RemoveAsset(std::shared_ptr<EditorAssetBase> pAsset);
	
	// Get the type of a specified class, should be the same as the C++ class/struct/enum name
	EditorTypeBase* FindTemplateType(const std::string& typeName) const; // Finds *all* types (struct/class/enum)
	EditorTypeClass* FindClassTemplateType(const std::string& typeName) const;
	std::vector<std::string> GetAllClassTemplateNames() const;
	EditorTypeStruct* FindStructTemplateType(const std::string& typeName) const;
	std::vector<std::string> GetAllStructTemplateNames() const;

	std::weak_ptr<EditorAssetBase> FindAsset(const std::string& typeName);

	bool IsEditorShowing() const { return bEditorShowing; }

	// Find the object with a given asset name. (i.e pass in "Health" and the object represented by Health.asset will be returned 
	template<typename T>
	T* FindObjectFromAsset(const std::string& name);

private:

	// #TODO: Need to seperate out asset management and imgui editor else the IMGUI_ENABLED ifdef will compile out assets
	
	// Generate template types. see templateClassTypes
	void CreateTemplateTypes(const std::string& typesFile);

	// Import assets from their files. see the assets map
	void ImportAssets(const std::string& assetsDirectory);

	void* FindObjectFromAssetInternal(const std::string& name);

	// Find template types from a given type map (struct, class, enum)
	EditorTypeBase* FindType(const std::string& typeName, const std::unordered_map<std::string, EditorTypeBase*>& templateTypes) const;
	std::vector<std::string> GetAllTypes(const std::unordered_map<std::string, EditorTypeBase*>& templateTypes) const;

	// This map will contain a single instance of each class type (the key being it's name) and all of the EditorTypes will have default values.
	// i.e "SpellFireball" -> "EditorTypeClass(SpellFireball) with all the default numbers"
	std::unordered_map<std::string, EditorTypeBase*> templateClassTypes;
	std::unordered_map<std::string, EditorTypeBase*> templateStructTypes;

	// This map will contain names to their assets. so it might be "Fireball" to a fireball asset, etc.
	std::unordered_map<std::string, std::shared_ptr<EditorAssetBase>> assets;
	
	// A list of windows currently showing.
	std::vector<std::shared_ptr<EditorWindowBase>> shownWindows;

	// List of all recently executed actions. Used for undo/redo
	std::vector<std::shared_ptr<EditorActionBase>> executedActions;
	int executedActionsIndex = -1; // The index of the last executed action. -1 means no actions have been executed yet.

	// Whether the editor is showing overlayed above the game -- if this is false don't do anything
	bool bEditorShowing = true;
};

template <typename T>
T* ImGuiEditor::FindObjectFromAsset(const std::string& name)
{
	// #TEMP: This needs to be a unique ptr or something, we need to figure out what's happening with object lifetimes (whether they're shared or instanced etc.)
	return static_cast<T*>(FindObjectFromAssetInternal(name));
}
