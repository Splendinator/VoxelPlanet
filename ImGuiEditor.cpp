#include "pch.h"

#ifdef DOMIMGUI

#include "ImGuiEditor.h"

#include "DomImport/DomImport.h"
#include "DomWindow/DomWindow.h"
#include "EditorActionBase.h"
#include "EditorAssetBase.h"
#include "EditorTypeBase.h"
#include "EditorTypeFactoryBase.h"
#include "EditorTypeFactoryClass.h"
#include "EditorWindowActionQueue.h"
#include "EditorWindowFilesystem.h"
#include "ImGuiEditorGlobals.h"

/// #TEMP: PRAGMA
#pragma optimize( "", off )

void ImGuiEditor::Init()
{	
	CreateTemplateTypes(ImGuiEditorGlobals::codeFilesBaseDirectory + "\\" + ImGuiEditorGlobals::editorTypesOutputFile);

	AddWindow(std::make_shared<EditorWindowFilesystem>(std::filesystem::path(ImGuiEditorGlobals::editorBaseDirectory)));
	AddWindow(std::make_shared<EditorWindowActionQueue>(executedActions, executedActionsIndex));
	
	bEditorShowing = false; // Start with editor off
}

void ImGuiEditor::Uninit()
{
	for (auto& [key, value] : templateTypes)
	{
		delete value;
	}	
	templateTypes.clear();

	for (auto& [key, value] : assets)
	{
		delete value;
	}
	assets.clear();
}

void ImGuiEditor::Tick()
{
	if (bEditorShowing)
	{
		// Draw windows
		for (std::shared_ptr<EditorWindowBase>& pWindow : shownWindows)
		{
			if (pWindow)
			{
				pWindow->Draw();
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
	}
	
	// Toggle editor
	if (dmwi::isPressed(dmwi::Button::F7))
	{
		bEditorShowing = !bEditorShowing;
	}
	
}

void ImGuiEditor::AddWindow(const std::shared_ptr<EditorWindowBase>& pWindow)
{
	DOMLOG_ERROR_IF(!bEditorShowing, "Shouldn't do anything when editor isn't showing");
	
	pWindow->Init(*this);

	shownWindows.push_back(pWindow);
}

void ImGuiEditor::RemoveWindow(EditorWindowBase* pWindow)
{
	DOMLOG_ERROR_IF(!bEditorShowing, "Shouldn't do anything when editor isn't showing");
	
	for (auto it = shownWindows.begin(); it != shownWindows.end(); ++it)
	{
		if (it->get() == pWindow)
		{
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

	if (pAction->TryExecuteAction())
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
		executedActions[executedActionsIndex]->Undo();
		--executedActionsIndex;
	}
}

void ImGuiEditor::Redo()
{
	DOMLOG_ERROR_IF(!bEditorShowing, "Shouldn't do anything when editor isn't showing");

	if (static_cast<int>(executedActions.size()) - 1 > executedActionsIndex)
	{
		if (executedActions[executedActionsIndex + 1]->TryExecuteAction())
		{
			++executedActionsIndex;
		}
		else
		{
			DOMLOG_ERROR("Cannot re-do an action? Something wrong?")
		}
	}
}

void ImGuiEditor::AddAsset(EditorAssetBase* pAsset)
{
	DOMLOG_ERROR_IF(!bEditorShowing, "Shouldn't do anything when editor isn't showing");

	assets.insert({ pAsset->GetName(), pAsset });
}

void ImGuiEditor::RemoveAsset(EditorAssetBase* pAsset)
{
	DOMLOG_ERROR_IF(!bEditorShowing, "Shouldn't do anything when editor isn't showing");

	assets.erase(pAsset->GetName());
	delete pAsset;
}

EditorTypeBase* ImGuiEditor::FindType(const std::string& typeName)
{
	auto it = templateTypes.find(typeName);
	if (it != templateTypes.end())
	{
		return it->second;
	}
	return nullptr;
}

std::vector<std::string> ImGuiEditor::GetAllTypes()
{
	std::vector<std::string> types;
	for (auto& [key, value] : templateTypes)
	{
		types.push_back(key);
	}
	
	std::sort(types.begin(), types.end());

	return types;
}

void ImGuiEditor::CreateTemplateTypes(const std::string& typesFile)
{
	std::ifstream inputFile(typesFile);

	EditorTypeFactoryClass editorTypeFactoryClass;
	
	EditorTypeFactoryBase* pEditorTypeFactories[] = 
	{
		&editorTypeFactoryClass
	};

	while (!inputFile.eof())
	{
		std::string nextString;
		inputFile >> nextString;

		if (nextString == "")
		{
			// Edge case - last line of file
			continue;
		}

		bool bFoundType = false;
		for (EditorTypeFactoryBase* pEditorFactory : pEditorTypeFactories)
		{
			if (pEditorFactory->GetKeyword() == nextString)
			{
				EditorTypeBase* pEditorType = pEditorFactory->CreateType(inputFile);

				DOMLOG_ERROR_IF(pEditorType == nullptr, "Failed to create editor type for type", pEditorFactory->GetKeyword());
				DOMLOG_ERROR_IF(pEditorType->name == "", "Editor type created with no name", pEditorFactory->GetKeyword());

				bFoundType = true;
				templateTypes.insert({ pEditorType->name, pEditorType });

				break;
			}
		}

		DOMLOG_ERROR_IF(!bFoundType, "Failed to find editor type factory for keyword", nextString);
	}
}

#endif //~ DOMIMGUI

#pragma optimize( "", on )