#include "pch.h"

#ifdef DOMIMGUI

#include "ImGuiEditor.h"

#include "DomImport/DomImport.h"
#include "DomWindow/DomWindow.h"
#include "Editor/Actions/EditorActionBase.h"
#include "Windows/EditorWindowActionQueue.h"
#include "Windows/EditorWindowFilesystem.h"
#include "ImGuiEditorGlobals.h"
#include "Input/InputContext.h"
#include "Input/InputSystem.h"

void ImGuiEditor::Init()
{
	AddWindow(std::make_shared<EditorWindowFilesystem>(std::filesystem::path(ImGuiEditorGlobals::editorBaseDirectory)));
	AddWindow(std::make_shared<EditorWindowActionQueue>(executedActions, executedActionsIndex));
	
	bEditorShowing = false; // Start with editor off

	// #JANK: If we ever resize this vector mid way through a loop (e.g clicking a button on one window to open a second window) then the game crashes.
	// #JANK: I can't be arsed to add a frame delay or whatever so I'm just reserving a bunch
	shownWindows.reserve(32); 
}

void ImGuiEditor::Uninit()
{
	if (bEditorShowing)
	{
		OnHideEditor();
	}
}

void ImGuiEditor::Tick(float deltaTime)
{
	if (bEditorShowing)
	{
		// Draw windows
		for (std::shared_ptr<EditorWindowBase>& pWindow : shownWindows)
		{
			if (pWindow)
			{
				pWindow->DrawBase();
			}
		}

		// Undo/redo
		if (dmwi::isHeld(dmwi::Button::CTRL))
		{
			if (dmwi::isPressed(dmwi::Button::Z))
			{
				Undo();
			}
			else if (dmwi::isPressed(dmwi::Button::Y))
			{
				Redo();
			}
		}

		// Close windows on ESC
		if (dmwi::isPressed(dmwi::Button::ESC))
		{
			for (std::shared_ptr<EditorWindowBase>& pWindow : shownWindows)
			{
				if (pWindow && pWindow->ShouldCloseOnRequest())
				{
					RemoveWindow(pWindow.get());
				}
			}
		}
	}
	
	// Toggle editor
	if (dmwi::isPressed(dmwi::Button::F7))
	{
		bEditorShowing = !bEditorShowing;

		if (pInputSystem && pEditorInputContext)
		{
			if (bEditorShowing)
			{
				OnShowEditor();		
			}
			else
			{
				OnHideEditor();
			}
		}
	}
}

void ImGuiEditor::AddWindow(const std::shared_ptr<EditorWindowBase>& pWindow)
{
	DOMLOG_ERROR_IF(!bEditorShowing, "Shouldn't do anything when editor isn't showing");

	// #JANK: I got a weird ass never before seen crash in domMain()/wWinMain() when I added this code. No idea what the fuck, but if it happens again remove this?
	// Remove any windows with the same name (ImGUI can't handle multiple windows with the same name and will just render everything on one window)
	for (auto it = shownWindows.begin(); it != shownWindows.end(); ++it)
	{
		if (it->get()->GetWindowName() == pWindow->GetWindowName())
		{
			RemoveWindow(it->get());
			break;
		}
	}
	
	pWindow->Init(*this);

	pWindow->OpenBase();

	shownWindows.push_back(pWindow);
}

void ImGuiEditor::RemoveWindow(EditorWindowBase* pWindow)
{
	DOMLOG_ERROR_IF(!bEditorShowing, "Shouldn't do anything when editor isn't showing");
	
	for (auto it = shownWindows.begin(); it != shownWindows.end(); ++it)
	{
		if (it->get() == pWindow)
		{
			pWindow->CloseBase();
			
			shownWindows.erase(it);
			return;
		}
	}
}

void ImGuiEditor::DoAction(const std::shared_ptr<EditorActionBase>& pAction)
{
	DOMLOG_ERROR_IF(!bEditorShowing, "Shouldn't do anything when editor isn't showing");

	// Remove all actions after the current action (i.e if you undo 4 times then do an action, you can't re-do the 4 actions you undid so we remove them)
	while (executedActionsIndex < static_cast<int>(executedActions.size()) - 1)
	{
		executedActions.pop_back();
	}

	if (pAction->BaseTryExecuteAction())
	{
		executedActions.push_back(pAction);
		executedActionsIndex = static_cast<int>(executedActions.size())-1;
	}
}

void ImGuiEditor::Undo()
{
	DOMLOG_ERROR_IF(!bEditorShowing, "Shouldn't do anything when editor isn't showing");
	
	if (executedActionsIndex > -1)
	{
		executedActions[executedActionsIndex]->BaseUndo();
		--executedActionsIndex;
	}
}

void ImGuiEditor::Redo()
{
	DOMLOG_ERROR_IF(!bEditorShowing, "Shouldn't do anything when editor isn't showing");

	if (static_cast<int>(executedActions.size()) - 1 > executedActionsIndex)
	{
		if (executedActions[executedActionsIndex + 1]->BaseTryExecuteAction())
		{
			++executedActionsIndex;
		}
		else
		{
			DOMLOG_ERROR("Cannot re-do an action? Something wrong?")
		}
	}
}

void ImGuiEditor::OnShowEditor()
{
	pInputSystem->PushInputContext(pEditorInputContext);
}

void ImGuiEditor::OnHideEditor()
{
	pInputSystem->PopInputContext(pEditorInputContext);
}

#endif //~ DOMIMGUI