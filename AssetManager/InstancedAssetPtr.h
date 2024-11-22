#pragma once
#include "AssetManager.h"
#include "EditorAssetBase.h"
#include "EditorTypeBase.h"
#include "Game.h"

class EditorAssetBase;

// This class points to an asset of an instanced object. From there it can create and delete an instance of that object.
// It's primary use case is when you want to load an asset on demand, e.g. in the menu you'd load up individual menu screens on the fly when opening the menu.
// This class is designed to be used with the EDITORPROPERTY() tag. It works similar to UClass in unreal. 
template<class T>
class TInstancedAssetPtr
{
public:

	TInstancedAssetPtr() = default;
	~TInstancedAssetPtr() { if (pInstance) Unload(); }

	T* operator->() {return pInstance;} 
	
	void SetAsset(const std::string& inAssetName)
	{
		DOMLOG_ERROR_IF(pInstance, "Why are you changing the asset when there's already an instance loaded from the old one?")
		
		assetName = inAssetName;
	}

	void Load();
	void Unload();
	

private:

	std::string assetName;
	T* pInstance = nullptr;
};

template <class T>
void TInstancedAssetPtr<T>::Load()
{
	DOMLOG_ERROR_IF(pInstance, "Loading while already loaded?")
	
	std::weak_ptr<EditorAssetBase> foundAsset = Game::GetAssetManager().FindAsset(assetName);
	if (foundAsset.expired())
	{
		DOMLOG_WARN("Loading invalid asset", assetName);
		return;
	}
	
	EditorAssetBase* pAsset = foundAsset.lock().get();
	DOMLOG_ERROR_IF(!pAsset->GetEditorType()->HasMetadataFlag(EClassMetadataFlags::Instanced), "This only works with instanced classes");
	
	pInstance = Game::GetAssetManager().LoadObjectFromAsset<T>(pAsset);
}

template <class T>
void TInstancedAssetPtr<T>::Unload()
{
	DOMLOG_ERROR_IF(pInstance == nullptr, "What the fuck are you trying to unload?")

	delete pInstance;
	pInstance = nullptr;
}
