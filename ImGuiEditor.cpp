#include "pch.h"

#ifdef DOMIMGUI

#include "ImGuiEditor.h"

#include "__Generated.h"
#include "DomImport/DomImport.h"
#include "DomWindow/DomWindow.h"
#include "EditorActionBase.h"
#include "EditorAssetBase.h"
#include "EditorAssetFactoryClass.h"
#include "EditorTypeBase.h"
#include "EditorTypeFactoryBase.h"
#include "EditorTypeFactoryClass.h"
#include "EditorWindowActionQueue.h"
#include "EditorWindowFilesystem.h"
#include "ImGuiEditorGlobals.h"

// #TEMP: Optimisation
#pragma optimize("", off)
void ImGuiEditor::Init()
{
	CreateTemplateTypes(ImGuiEditorGlobals::codeFilesBaseDirectory + "\\" + ImGuiEditorGlobals::editorTypesOutputFile);

	ImportAssets(ImGuiEditorGlobals::editorBaseDirectory);
	
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
	}
	
}


void ImGuiEditor::AddWindow(const std::shared_ptr<EditorWindowBase>& pWindow)
{
	DOMLOG_ERROR_IF(!bEditorShowing, "Shouldn't do anything when editor isn't showing");
	
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

void ImGuiEditor::AddAsset(std::shared_ptr<EditorAssetBase> pAsset)
{
	DOMLOG_ERROR_IF(!bEditorShowing, "Shouldn't do anything when editor isn't showing");

	assets.insert({ pAsset->GetName(), pAsset });
}

void ImGuiEditor::RemoveAsset(std::shared_ptr<EditorAssetBase> pAsset)
{
	DOMLOG_ERROR_IF(!bEditorShowing, "Shouldn't do anything when editor isn't showing");

	assets.erase(pAsset->GetName());
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

std::vector<std::string> ImGuiEditor::GetAllTypes() const
{
	std::vector<std::string> types;
	for (auto& [key, value] : templateTypes)
	{
		types.push_back(key);
	}
	
	std::sort(types.begin(), types.end());

	return types;
}

std::weak_ptr<EditorAssetBase> ImGuiEditor::FindAsset(const std::string& typeName)
{
	auto it = assets.find(typeName);
	if (it != assets.end())
	{
		return it->second;
	}

	DOMLOG_ERROR("Asset", typeName, "not found")
	
	return {};
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

void ImGuiEditor::ImportAssets(const std::string& assetsDirectory)
{
	EditorAssetFactoryClass editorAssetFactoryClass(*this);

	EditorAssetFactoryBase* pAssetFactories[] =
	{
		&editorAssetFactoryClass
	};

	std::filesystem::recursive_directory_iterator dirIter(assetsDirectory), end;

	while (dirIter != end)
	{
		if (dirIter->is_regular_file() && dirIter->path().extension() == ImGuiEditorGlobals::assetExtension)
		{
			std::ifstream assetFile(dirIter->path());
			if (assetFile.is_open())
			{
				std::string keyword;
				assetFile >> keyword;
				for (EditorAssetFactoryBase* pAssetFactory : pAssetFactories)
				{
					if (pAssetFactory->GetKeyword() == keyword)
					{
						AddAsset(pAssetFactory->CreateAsset(dirIter->path()));
						break;
					}
				}
			}
		}

		try
		{
			++dirIter;
		}
		catch (const std::filesystem::filesystem_error& e)
		{
			// This can fail due to permissions, etc.
			DOMLOG_ERROR("Failed to iterate directory", e.what());
			return;
		}
	}
}

void* ImGuiEditor::FindObjectInternal(const std::string& name)
{
	std::weak_ptr<EditorAssetBase> pAsset = FindAsset(name);

	if (!pAsset.expired())
	{
		auto it = __Generated::stringToCreateClassFunction.find(pAsset.lock()->GetEditorType()->name);

		if (it != __Generated::stringToCreateClassFunction.end())
		{
			return it->second(pAsset.lock().get());
		}
	}

	DOMLOG_ERROR("Object", name, "not found");
	return nullptr;
}

#endif //~ DOMIMGUI
#pragma optimize("", on)