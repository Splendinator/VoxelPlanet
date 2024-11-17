#pragma once

#include "Core/GameSystem.h"

class EditorActionBase;
class EditorWindowBase;

/** ImGuiEditor
*
* This class is used to edit the game in using ImGui. 
*
* It reads in all the code files in the project and parses them for classes, structs, enums, etc and lets you edit them.
*/
EDITORCLASS()
class ImGuiEditor : public GameSystem
{
	EDITORBODY()
	
public:
	ImGuiEditor() {}

	//~ Begin GameSystem Interface
	void Init();
	void Tick(float deltaTime);
	void Uninit();
	//~ End GameSystem Interface
	
	void AddWindow(const std::shared_ptr<EditorWindowBase>& pWindow);
	void RemoveWindow(EditorWindowBase* pWindow);

	void DoAction(const std::shared_ptr<EditorActionBase>& pAction);
	void Undo(); // Undo the last action
	void Redo(); // Redo the last undone action if possible
	
	bool IsEditorShowing() const { return bEditorShowing; }

private:
	
	// A list of windows currently showing.
	std::vector<std::shared_ptr<EditorWindowBase>> shownWindows;

	// List of all recently executed actions. Used for undo/redo
	std::vector<std::shared_ptr<EditorActionBase>> executedActions;
	int executedActionsIndex = -1; // The index of the last executed action. -1 means no actions have been executed yet.

	// Whether the assetManager is showing overlayed above the game -- if this is false don't do anything
	bool bEditorShowing = true;
};
