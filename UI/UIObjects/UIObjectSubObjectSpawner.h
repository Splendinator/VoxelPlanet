#pragma once

#include "UIObjectBase.h"

#include "UI/HUD/HUDAnchorPoint.h"

enum class EUIObjectSubObjectSpawnerSpawnDirection
{
	Horizontal,
	Vertical,
};

enum class EUIObjectSubObjectSpawnerSpawnLayout
{
	// EUIObjectSubObjectSpawnerSpawnDirection::Horizontal
	Top,
	Bottom,

	// EUIObjectSubObjectSpawnerSpawnDirection::Vertical
	Left,
	Right,
	
	// Both
	Center,
};

struct UIObjectSubObjectSpawnerSpawnParams
{
	// Name of the file / layer containing the object to load in.
	std::string file;
	std::string layerName;

	// Number of the objects to load in
	u32 numObjects;

	// Direction + layout of spawned objects
	EUIObjectSubObjectSpawnerSpawnDirection spawnDirection = EUIObjectSubObjectSpawnerSpawnDirection::Horizontal;
	EUIObjectSubObjectSpawnerSpawnLayout spawnLayout = EUIObjectSubObjectSpawnerSpawnLayout::Center;
	
};

// Class used to dynamically spawn subobjects into a given layout
class UIObjectSubObjectSpawner : public UIObjectBase
{
public:


	// Returns false if there was an error
	bool TrySpawnSubobjects(const UIObjectSubObjectSpawnerSpawnParams& spawnParams);

	template<class TUIObject>
	std::vector<TransientPtr<TUIObject>> SpawnSubobjectsAs(const UIObjectSubObjectSpawnerSpawnParams& spawnParams);
	
	
protected:

	//~ Begin UIObjectBase Interface
	void Init(VectorPrimitiveLayer* pRoot) override;
	void Uninit() override;
	//~ End UIObjectBase Interface

	TransientPtr<VectorPrimitiveLayer> pSpawnArea = nullptr;
};

template <class TUIObject>
std::vector<TransientPtr<TUIObject>> UIObjectSubObjectSpawner::SpawnSubobjectsAs(const UIObjectSubObjectSpawnerSpawnParams& spawnParams)
{
	std::vector<TransientPtr<TUIObject>> returnedObjects;
	
	if (TrySpawnSubobjects(spawnParams))
	{
		returnedObjects.reserve(spawnParams.numObjects);
		for (VectorPrimitiveBase* pNewlySpawnedObjectRoot : pSpawnArea->GetChildren())
		{
			returnedObjects.push_back(AddUIObject<TUIObject>(static_cast<VectorPrimitiveLayer*>(pNewlySpawnedObjectRoot)));
		}
	}

	return returnedObjects;
}
