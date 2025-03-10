#pragma once

#include "EditorTypeBase.h"

class EditorAssetBase;
class EditorTypeBase;
class EditorTypeClass;
class EditorTypeStruct;
class EditorTypeEnum;

#define ENUMSTRING(enum, value) ((int)enum)/*will compile error if misspelled or non-enum type*/, Game::GetAssetManager().GetEnumValueNameFromValue(#enum, (int)value))

// #TODO: Maybe look into a way to extend _DeleteObject to automatically delete member instanced pointers

/** Asset Manager
 *
 * This class is responsible for eveything assets: reading them in, instantiating objects from them, etc.
 */
class AssetManager
{
public:

	void Init();
	void UnInit();
	
	void AddAsset(std::shared_ptr<EditorAssetBase> pAsset);
	void RemoveAsset(std::shared_ptr<EditorAssetBase> pAsset);
	
	// There is one template for each class/struct/enum. They are created with all of the default values set up,
	// so if someone makes a struct with a member "float health = 2.0f" the template EditorTypeStruct will have a EditorTypePropertyFloat with value 2.0f.
	// They are deep copied when you create a new asset of a given type such that the asset spawns with all the correct default values.
	EditorTypeBase* FindTemplateType(const std::string& typeName) const; // Finds *all* types (struct/class/enum)
	EditorTypeClass* FindClassTemplateType(const std::string& typeName) const;
	std::vector<std::string> GetAllClassTemplateNames(bool bIgnoreAbstract) const;
	EditorTypeStruct* FindStructTemplateType(const std::string& typeName) const;
	std::vector<std::string> GetAllStructTemplateNames(bool bIgnoreAbstract) const;

	// Gather the names of all child class templates that derrive from className (including className) optionally ignoring abstract classes
	std::vector<std::string> GetAllChildClassTemplateNames(std::string className, bool bIgnoreAbstract) const;

	std::weak_ptr<EditorAssetBase> FindAsset(const std::string& assetName) const;

	// Gather all assets of a given class
	// bGatherChildClasses - whether to also gather classes that are children of className
	// RequiredFlags - Flags the asset must have (useful for only getting singletons etc.)
	std::vector<std::weak_ptr<EditorAssetBase>> GatherAssetsOfClass(const std::string& className, bool bGatherChildClasses, EClassMetadataFlags requiredFlags = EClassMetadataFlags::None) const;

	// Enum utils
	std::string GetEnumValueNameFromValue(const std::string& enumName, int value) const;
	int GetEnumValueFromValueName(const std::string& enumName, const std::string& valueName) const;
	EditorTypeEnum* FindEnumType(const std::string& enumName) const;

	// Find the object with a given asset name. (i.e pass in "Health" and the object represented by Health.asset will be returned 
	template<typename T>
	T* LoadObjectFromAssetName(const std::string& name);
	template<typename T>
	T* LoadObjectFromAsset(EditorAssetBase* pAsset);
	
	// Get editor name from object if possible, this is slow so just use it for debug.
	// Right now this can only be done for singleton objects
	std::string FindNameFromSingletonSlow(void* pObject);
	
private:
	
	// Generate template types. see templateClassTypes
	void CreateTemplateTypes(const std::string& typesFile);

	// Import assets from their files. see the assets map
	void ImportAssets(const std::string& assetsDirectory);

	void* LoadObjectFromAssetInternal(EditorAssetBase* pAsset);

	// Find template types from a given type map (struct, class, enum)
	EditorTypeBase* FindType(const std::string& typeName, const std::unordered_map<std::string, EditorTypeBase*>& templateTypes) const;

	// Get all types from a given map type. bIgnoreAbstract to ignore any with the EClassMetadata::Abstract flag
	std::vector<std::string> GetAllTypes(const std::unordered_map<std::string, EditorTypeBase*>& templateTypes, bool bIgnoreAbstract) const;

	// This map will contain a single instance of each class type (the key being it's name) and all of the EditorTypes will have default values.
	// i.e "SpellFireball" -> "EditorTypeClass(SpellFireball) with all the default numbers"
	std::unordered_map<std::string, EditorTypeBase*> templateClassTypes; // EditorTypeClass
	std::unordered_map<std::string, EditorTypeBase*> templateStructTypes; // EditorTypeStruct
	std::unordered_map<std::string, EditorTypeBase*> templateEnumTypes; // EditorTypeEnum

	// This map will contain names to their assets. so it might be "Fireball" to a fireball asset, etc.
	std::unordered_map<std::string, std::shared_ptr<EditorAssetBase>> assets;

	// Map of assets to their singletons (if the asset represents a class with the Singleton EClassMetadataFlags)
	std::unordered_map<EditorAssetBase*, void*> singletonMap;
};

template <typename T>
T* AssetManager::LoadObjectFromAssetName(const std::string& name)
{
	auto it = assets.find(name);
	if (it != assets.end())
	{
		return static_cast<T*>(LoadObjectFromAssetInternal(it->second.get()));
	}
	
	DOMLOG_WARN("No asset with name", name)
	return nullptr;
}

template <typename T>
T* AssetManager::LoadObjectFromAsset(EditorAssetBase* pAsset)
{
	return static_cast<T*>(LoadObjectFromAssetInternal(pAsset));
}
