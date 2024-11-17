#include "pch.h"

#include "AssetManager.h"

#include "EditorAssetBase.h"
#include "EditorAssetClass.h"
#include "EditorAssetFactoryClass.h"
#include "EditorTypeBase.h"
#include "EditorTypeClass.h"
#include "EditorTypeEnum.h"
#include "EditorTypeFactoryBase.h"
#include "EditorTypeFactoryClass.h"
#include "EditorTypeFactoryEnum.h"
#include "EditorTypeFactoryStruct.h"
#include "EditorTypeStruct.h"
#include "ImGuiEditorGlobals.h"
#include "__Generated.h"

void AssetManager::Init()
{
	CreateTemplateTypes(ImGuiEditorGlobals::codeFilesBaseDirectory + "\\" + ImGuiEditorGlobals::editorTypesOutputFile);

	ImportAssets(ImGuiEditorGlobals::editorBaseDirectory);	
}

void AssetManager::UnInit()
{
	for (auto& [key, value] : templateClassTypes)
	{
		delete value;
	}	
	templateClassTypes.clear();

	assets.clear();
}

void AssetManager::AddAsset(std::shared_ptr<EditorAssetBase> pAsset)
{
	//DOMLOG_ERROR_IF(!bEditorShowing, "Shouldn't do anything when ditor isn't showing");

	assets.insert({ pAsset->GetName(), pAsset });
}

void AssetManager::RemoveAsset(std::shared_ptr<EditorAssetBase> pAsset)
{
	//DOMLOG_ERROR_IF(!bEditorShowing, "Shouldn't do anything when editor isn't showing");

	assets.erase(pAsset->GetName());
}

EditorTypeBase* AssetManager::FindTemplateType(const std::string& typeName) const
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
	
	DOMASSERT(false); // #TODO: Handle enums here too 
	return nullptr;
}

EditorTypeClass* AssetManager::FindClassTemplateType(const std::string& typeName) const
{
	return static_cast<EditorTypeClass*>(FindType(typeName, templateClassTypes));
}

std::vector<std::string> AssetManager::GetAllClassTemplateNames(bool bIgnoreAbstract) const
{
	return GetAllTypes(templateClassTypes, bIgnoreAbstract);
}

EditorTypeStruct* AssetManager::FindStructTemplateType(const std::string& typeName) const
{
	return static_cast<EditorTypeStruct*>(FindType(typeName, templateStructTypes));
}

std::vector<std::string> AssetManager::GetAllStructTemplateNames(bool bIgnoreAbstract) const
{
	return GetAllTypes(templateStructTypes, bIgnoreAbstract);
}

std::weak_ptr<EditorAssetBase> AssetManager::FindAsset(const std::string& typeName) const
{
	auto it = assets.find(typeName);
	if (it != assets.end())
	{
		return it->second;
	}

	DOMLOG_ERROR("Asset", typeName, "not found")
	
	return {};
}

std::vector<std::weak_ptr<EditorAssetBase>> AssetManager::GatherAssetsOfClass(const std::string& className, bool bGatherChildClasses) const
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

std::string AssetManager::GetEnumValueNameFromValue(const std::string& enumName, int value) const
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

int AssetManager::GetEnumValueFromValueName(const std::string& enumName, const std::string& valueName) const
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

EditorTypeEnum* AssetManager::FindEnumType(const std::string& enumName) const
{
	return static_cast<EditorTypeEnum*>(FindType(enumName, templateEnumTypes));
}

void AssetManager::CreateTemplateTypes(const std::string& typesFile)
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

void AssetManager::ImportAssets(const std::string& assetsDirectory)
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

void* AssetManager::FindObjectFromAssetInternal(const std::string& name)
{
	std::weak_ptr<EditorAssetBase> pAsset = FindAsset(name);
	
	if (!pAsset.expired())
	{
		EditorAssetClass* pClassAsset = dynamic_cast<EditorAssetClass*>(pAsset.lock().get());
		DOMLOG_ERROR_IF(pClassAsset == nullptr, "Right now we only support class assets");		

		if (pClassAsset->GetEditorType()->HasMetadataFlag(EClassMetadataFlags::Instanced))
		{
			// Instanced -- Always create a new object using the __generated cpp function
			auto it = __Generated::stringToCreateObjectFunction.find(pClassAsset->GetEditorType()->name);
			if (it != __Generated::stringToCreateObjectFunction.end())
			{
				return it->second(pClassAsset->GetProperties());
			}
		}
		else
		{
			// Singleton -- Create a new object using the __generated cpp function the first time, then always return that.
			
			DOMASSERT(pClassAsset->GetEditorType()->HasMetadataFlag(EClassMetadataFlags::Singleton)) // Must have at least 1 instancing flag

			auto foundSingleton = singletonMap.find(pClassAsset);
			if (foundSingleton == singletonMap.end())
			{
				auto it = __Generated::stringToCreateObjectFunction.find(pClassAsset->GetEditorType()->name);
				if (it != __Generated::stringToCreateObjectFunction.end())
				{
					void* newSingleton = it->second(pClassAsset->GetProperties());
					singletonMap.emplace(pClassAsset, newSingleton);
					return newSingleton;
				}
			}
			return foundSingleton->second;
		}
	}

	DOMLOG_ERROR("Object", name, "not found");
	return nullptr;
}

EditorTypeBase* AssetManager::FindType(const std::string& typeName, const std::unordered_map<std::string, EditorTypeBase*>& templateTypes) const
{
	auto it = templateTypes.find(typeName);
	if (it != templateTypes.end())
	{
		return it->second;
	}
	return nullptr;
}

std::vector<std::string> AssetManager::GetAllTypes(const std::unordered_map<std::string, EditorTypeBase*>& templateTypes, bool bIgnoreAbstract) const
{
	std::vector<std::string> types;
	for (auto& [key, value] : templateTypes)
	{
		if (!bIgnoreAbstract || !value->HasMetadataFlag(EClassMetadataFlags::Abstract))
		{
			types.push_back(key);
		}
	}
	
	std::sort(types.begin(), types.end());

	return types;
}
