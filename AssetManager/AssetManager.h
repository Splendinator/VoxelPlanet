#pragma once

class EditorAssetBase;
class EditorTypeBase;
class EditorTypeClass;
class EditorTypeStruct;
class EditorTypeEnum;

class AssetManager
{
public:

	void Init();
	void UnInit();
	
	void AddAsset(std::shared_ptr<EditorAssetBase> pAsset);
	void RemoveAsset(std::shared_ptr<EditorAssetBase> pAsset);
	
	// Get the type of a specified class, should be the same as the C++ class/struct/enum name
	EditorTypeBase* FindTemplateType(const std::string& typeName) const; // Finds *all* types (struct/class/enum)
	EditorTypeClass* FindClassTemplateType(const std::string& typeName) const;
	std::vector<std::string> GetAllClassTemplateNames(bool bIgnoreAbstract) const;
	EditorTypeStruct* FindStructTemplateType(const std::string& typeName) const;
	std::vector<std::string> GetAllStructTemplateNames(bool bIgnoreAbstract) const;

	std::weak_ptr<EditorAssetBase> FindAsset(const std::string& typeName) const;
	std::vector<std::weak_ptr<EditorAssetBase>> GatherAssetsOfClass(const std::string& className, bool bGatherChildClasses) const;

	// Enum utils
	std::string GetEnumValueNameFromValue(const std::string& enumName, int value) const;
	int GetEnumValueFromValueName(const std::string& enumName, const std::string& valueName) const;
	EditorTypeEnum* FindEnumType(const std::string& enumName) const;
	
	// Find the object with a given asset name. (i.e pass in "Health" and the object represented by Health.asset will be returned 
	template<typename T>
	T* FindObjectFromAsset(const std::string& name);
	
private:
	
	// Generate template types. see templateClassTypes
	void CreateTemplateTypes(const std::string& typesFile);

	// Import assets from their files. see the assets map
	void ImportAssets(const std::string& assetsDirectory);

	void* FindObjectFromAssetInternal(const std::string& name);

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
T* AssetManager::FindObjectFromAsset(const std::string& name)
{
	return static_cast<T*>(FindObjectFromAssetInternal(name));
}
