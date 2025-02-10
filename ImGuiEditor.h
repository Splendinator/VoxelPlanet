#pragma once

#include "Core/GameSystem.h"

class EditorActionBase;
class EditorWindowBase;
class InputContext;
class InputSystem;

/** ImGuiEditor
*
* This class is used to edit assets in-game in using ImGui.
* The ImGui editor is editor exposed which is fucking stupid but it makes me happy.
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

	void OnShowEditor();
	void OnHideEditor();
	
	// Input system
	EDITORPROPERTY()
	InputSystem* pInputSystem = nullptr;

	// Input context to push while editor is open
	EDITORPROPERTY()
	InputContext* pEditorInputContext = nullptr;
	
	// A list of windows currently showing.
	std::vector<std::shared_ptr<EditorWindowBase>> shownWindows;

	// List of all recently executed actions. Used for undo/redo
	std::vector<std::shared_ptr<EditorActionBase>> executedActions;
	int executedActionsIndex = -1; // The index of the last executed action. -1 means no actions have been executed yet.

	// Whether the assetManager is showing overlayed above the game -- if this is false don't do anything
	bool bEditorShowing = true;
};
