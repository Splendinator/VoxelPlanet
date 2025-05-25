#pragma once

#include "AssetManager.h"
#include "Editor/Assets/EditorAssetClass.h"
#include "Editor/Types/EditorTypeClass.h"
#include "Game.h"

class EditorAssetBase;

// This class points to an asset of an instanced object. From there it can create and delete an instance of that object.
// It's primary use case is when you want to load an asset on demand, e.g. in the menu you'd load up individual menu screens on the fly when opening the menu.
// This class is designed to be used with the EDITORPROPERTY() tag. It works similar to UClass in unreal. 
template<class T>
class InstancedAssetPtr
{
public:

	InstancedAssetPtr() = default;
	~InstancedAssetPtr() { if (pInstance) Unload(); }
	InstancedAssetPtr(const InstancedAssetPtr<T>& other)
	{
		assetName = other.assetName;
		DOMLOG_ERROR_IF(other.pInstance, "Trying to copy instanced asset pointer with loaded instance, This won't work as instance is deleted in destructor. Try move semantics.")
	}
	InstancedAssetPtr(InstancedAssetPtr<T>&& other)
	{
		assetName = std::move(other.assetName);
		pInstance = other.pInstance;
		other.pInstance = nullptr;
	};

	T* operator->() {return pInstance;} 
	
	void SetAsset(const std::string& inAssetName)
	{
		DOMLOG_ERROR_IF(pInstance, "Why are you changing the asset when there's already an instance loaded from the old one?")
		
		assetName = inAssetName;
	}

	T* Get() const { return pInstance; }
	
	void Load();
	void Unload();

	// Make a unique ptr using the instanced asset as a template.
	std::unique_ptr<T> MakeUnique();

	bool IsLoaded() const { return pInstance != nullptr; }

	// This isn't checking if the assetName actually resolves to a valid asset
	bool IsAssetSet() const { return assetName != "nullptr" && assetName != ""; }
	

private:

	std::string assetName;
	T* pInstance = nullptr;
};

template <class T>
void InstancedAssetPtr<T>::Load()
{
	DOMLOG_ERROR_IF(pInstance, "Loading while already loaded?")
	
	std::weak_ptr<EditorAssetClass> foundAsset = Game::GetAssetManager().FindAsset<EditorAssetClass>(assetName);
	if (foundAsset.expired())
	{
		DOMLOG_WARN("Loading invalid asset", assetName);
		return;
	}
	
	EditorAssetClass* pAsset = foundAsset.lock().get();
	DOMLOG_ERROR_IF(!pAsset->GetClassEditorType()->HasMetadataFlag(EClassMetadataFlags::Instanced), "This only works with instanced classes");
	
	pInstance = Game::GetAssetManager().LoadObjectFromClassAsset<T>(pAsset);
}

template <class T>
void InstancedAssetPtr<T>::Unload()
{
	DOMLOG_ERROR_IF(pInstance == nullptr, "What the fuck are you trying to unload?")

	delete pInstance;
	pInstance = nullptr;
}

template <class T>
std::unique_ptr<T> InstancedAssetPtr<T>::MakeUnique()
{
	std::unique_ptr<T> pUniquePtr;

	if (pInstance == nullptr)
	{
		Load();
	}
	
	if (pInstance != nullptr)
	{
		pUniquePtr.reset(pInstance);
		pInstance = nullptr; // We don't own the instance any more
	}

	return pUniquePtr;
}
