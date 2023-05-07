#pragma once

class CodeParseTokenBase;
class EditorActionBase;
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

private:
	
	// Generate template types. see templateTypes
	void CreateTemplateTypes(const std::string& typesFile);

	// This map will contain a single instance of each type (the key being it's name) and all of the EditorTypes will have default values.
	// i.e "SpellFireball" -> "EditorTypeClass(SpellFireball) with all the default numbers"
	std::unordered_map<std::string, EditorTypeBase*> templateTypes;
	
	// A list of windows currently showing.
	std::vector<std::shared_ptr<EditorWindowBase>> shownWindows;

	// List of all recently executed actions. Used for undo/redo
	std::vector<std::shared_ptr<EditorActionBase>> executedActions;
	int executedActionsIndex = -1; // The index of the last executed action. -1 means no actions have been executed yet.
};
