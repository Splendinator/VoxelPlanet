#include "pch.h"

#include "EditorTypeEnum.h"

#ifdef DOMIMGUI

#include "ImGuiEditor.h"

#include "__Generated.h"
#include "DomImport/DomImport.h"
#include "DomWindow/DomWindow.h"
#include "EditorActionBase.h"
#include "EditorAssetBase.h"
#include "EditorAssetClass.h"
#include "EditorAssetFactoryClass.h"
#include "EditorTypeBase.h"
#include "EditorTypeClass.h"
#include "EditorTypeFactoryBase.h"
#include "EditorTypeFactoryClass.h"
#include "EditorTypeFactoryStruct.h"
#include "EditorTypeFactoryEnum.h"
#include "EditorTypeStruct.h"
#include "EditorWindowActionQueue.h"
#include "EditorWindowFilesystem.h"
#include "ImGuiEditorGlobals.h"

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
	for (auto& [key, value] : templateClassTypes)
	{
		delete value;
	}	
	templateClassTypes.clear();

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

EditorTypeBase* ImGuiEditor::FindTemplateType(const std::string& typeName) const
{
	EditorTypeBase* pFoundType = nullptr;

	pFoundType = FindClassTemplateType(typeName);
	if (pFoundType)
	{
		return pFoundType;
	}

	pFoundType = FindStructTemplateType(typeName);
	if (pFoundType)
	{
		return pFoundType;
	}

	// #TODO: Handle enums here too 
}

EditorTypeClass* ImGuiEditor::FindClassTemplateType(const std::string& typeName) const
{
	return static_cast<EditorTypeClass*>(FindType(typeName, templateClassTypes));
}

std::vector<std::string> ImGuiEditor::GetAllClassTemplateNames() const
{
	return GetAllTypes(templateClassTypes);
}

EditorTypeStruct* ImGuiEditor::FindStructTemplateType(const std::string& typeName) const
{
	return static_cast<EditorTypeStruct*>(FindType(typeName, templateStructTypes));
}

std::vector<std::string> ImGuiEditor::GetAllStructTemplateNames() const
{
	return GetAllTypes(templateStructTypes);
}

std::weak_ptr<EditorAssetBase> ImGuiEditor::FindAsset(const std::string& typeName) const
{
	auto it = assets.find(typeName);
	if (it != assets.end())
	{
		return it->second;
	}

	DOMLOG_ERROR("Asset", typeName, "not found")
	
	return {};
}

// #TEMP: Optimisation
#pragma optimize("", off)
std::vector<std::weak_ptr<EditorAssetBase>> ImGuiEditor::GatherAssetsOfClass(const std::string& className, bool bGatherChildClasses) const
{
	std::vector<std::weak_ptr<EditorAssetBase>> gatheredAssets;

	std::vector<std::string> classNamesToGather = {className};

	// Gather child classes (i.e "HUDObjectBase" should gather all HUDObjects "HUDObjectHealth" etc.)
	if (bGatherChildClasses)
	{
		std::vector<std::string> classNamesOne = { className };
		std::vector<std::string> classNamesTwo = {};
		std::vector<std::string>* pLastVector = &classNamesOne;
		std::vector<std::string>* pCurrentVector = &classNamesTwo;

		// Repeatedly gather base classes one level at a time until all base classes have been gathered.  
		while (pLastVector->size() > 0)
		{
			for (auto& it : templateClassTypes)
			{
				EditorTypeClass* pEditorClass = static_cast<EditorTypeClass*>(it.second);

				if (std::find(classNamesToGather.begin(), classNamesToGather.end(), pEditorClass->name) == classNamesToGather.end())
				{
					for (std::string& baseClass : pEditorClass->baseClasses)
					{
						if (std::find(pLastVector->begin(), pLastVector->end(), baseClass) != pLastVector->end())
						{
							classNamesToGather.push_back(pEditorClass->name);
							pCurrentVector->push_back(pEditorClass->name);
						}
					}
				}
			}

			pLastVector->clear();
			std::swap(pCurrentVector, pLastVector);
		}
	}
	
	for (auto& asset : assets)
	{
		if (EditorTypeClass* pEditorClass = dynamic_cast<EditorTypeClass*>(asset.second->GetEditorType()))
		{
			if (std::find(classNamesToGather.begin(), classNamesToGather.end(), pEditorClass->name) != classNamesToGather.end())
			{
				gatheredAssets.push_back(asset.second);
			}
		}
	}

	return gatheredAssets;
}
#pragma optimize("", on)

std::string ImGuiEditor::GetEnumValueNameFromValue(const std::string& enumName, int value) const
{
	auto it = templateEnumTypes.find(enumName);
	if (it != templateEnumTypes.end())
	{
		EditorTypeEnum* pEnumType = static_cast<EditorTypeEnum*>(it->second);

		for(EnumValueNameToValue& valueNameToValue : pEnumType->valueNamesToValues)
		{
			if (valueNameToValue.value == value)
			{
				return valueNameToValue.valueName;
			}
		}
	}

	DOMLOG_WARN("Cannot find enum value", enumName, value);
	return "";
}

int ImGuiEditor::GetEnumValueFromValueName(const std::string& enumName, const std::string& valueName) const
{
	auto it = templateEnumTypes.find(enumName);
	if (it != templateEnumTypes.end())
	{
		EditorTypeEnum* pEnumType = static_cast<EditorTypeEnum*>(it->second);

		for(EnumValueNameToValue& valueNameToValue : pEnumType->valueNamesToValues)
		{
			if (valueNameToValue.valueName == valueName)
			{
				return valueNameToValue.value;
			}
		}
	}

	DOMLOG_WARN("Cannot find enum value", enumName, valueName);
	return 0;
}

EditorTypeEnum* ImGuiEditor::FindEnumType(const std::string& enumName) const
{
	return static_cast<EditorTypeEnum*>(FindType(enumName, templateEnumTypes));
}

void ImGuiEditor::CreateTemplateTypes(const std::string& typesFile)
{
	std::ifstream inputFile(typesFile);
	
	struct EditorTypeToTemplateMap
	{
		EditorTypeFactoryBase* pFactory;
		std::unordered_map<std::string, EditorTypeBase*>& templateTypeMap; 
	};
	
	EditorTypeFactoryClass editorTypeFactoryClass;
	EditorTypeFactoryStruct editorTypeFactoryStruct;
	EditorTypeFactoryEnum editorTypeFactoryEnum;
	
	EditorTypeToTemplateMap editorTypeFactories[] = 
	{
		{ &editorTypeFactoryClass, templateClassTypes},
		{ &editorTypeFactoryStruct, templateStructTypes},
		{ &editorTypeFactoryEnum, templateEnumTypes},
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
		for (EditorTypeToTemplateMap& editorTypeToTemplateMap : editorTypeFactories)
		{
			if (editorTypeToTemplateMap.pFactory->GetKeyword() == nextString)
			{
				EditorTypeBase* pEditorType = editorTypeToTemplateMap.pFactory->CreateType(inputFile);

				DOMLOG_ERROR_IF(pEditorType == nullptr, "Failed to create editor type for type", editorTypeToTemplateMap.pFactory->GetKeyword());
				DOMLOG_ERROR_IF(pEditorType->name == "", "Editor type created with no name", editorTypeToTemplateMap.pFactory->GetKeyword());

				bFoundType = true;
				editorTypeToTemplateMap.templateTypeMap.insert({ pEditorType->name, pEditorType });

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
						if (std::shared_ptr<EditorAssetBase> asset = pAssetFactory->CreateAsset(dirIter->path()))
						{
							AddAsset(asset);
							break;
						}
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

void* ImGuiEditor::FindObjectFromAssetInternal(const std::string& name)
{
	std::weak_ptr<EditorAssetBase> pAsset = FindAsset(name);
	
	if (!pAsset.expired())
	{
		EditorAssetClass* pClassAsset = dynamic_cast<EditorAssetClass*>(pAsset.lock().get());
		DOMLOG_ERROR_IF(pClassAsset == nullptr, "Right now we only support class assets");
		
		auto it = __Generated::stringToCreateObjectFunction.find(pClassAsset->GetEditorType()->name);

		if (it != __Generated::stringToCreateObjectFunction.end())
		{
			return it->second(pClassAsset->GetProperties());
		}
	}

	DOMLOG_ERROR("Object", name, "not found");
	return nullptr;
}

EditorTypeBase* ImGuiEditor::FindType(const std::string& typeName, const std::unordered_map<std::string, EditorTypeBase*>& templateTypes) const
{
	auto it = templateTypes.find(typeName);
	if (it != templateTypes.end())
	{
		return it->second;
	}
	return nullptr;
}

std::vector<std::string> ImGuiEditor::GetAllTypes(const std::unordered_map<std::string, EditorTypeBase*>& templateTypes) const
{
	std::vector<std::string> types;
	for (auto& [key, value] : templateTypes)
	{
		types.push_back(key);
	}
	
	std::sort(types.begin(), types.end());

	return types;
}

#endif //~ DOMIMGUI