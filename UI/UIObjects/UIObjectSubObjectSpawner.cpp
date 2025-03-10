#include "pch.h"

#include "UIObjectSubObjectSpawner.h"

#include "VectorArt.h"

void UIObjectSubObjectSpawner::Init(VectorPrimitiveLayer* pRoot)
{
	pSpawnArea = pRoot;
}

void UIObjectSubObjectSpawner::Uninit()
{
	
}

bool UIObjectSubObjectSpawner::TrySpawnSubobjects(const UIObjectSubObjectSpawnerSpawnParams& spawnParams)
{
	std::unique_ptr<VectorArt> pLoadedVectorArt(new VectorArt(spawnParams.file.c_str()));
	VectorPrimitiveLayer* pLoadedInLayer = pLoadedVectorArt->FindLayerByLabel(spawnParams.layerName);
	if (pLoadedInLayer == nullptr)
	{
		return false;
	}
	
	pSpawnArea->ClearChildren();
	
	for (u32 spawnedSubObjectIndex = 0; spawnedSubObjectIndex < spawnParams.numObjects; spawnedSubObjectIndex++)
	{
		//VectorPrimitiveLayer* pSpawnedLayer = static_cast<VectorPrimitiveLayer*>(pLoadInLayer->DeepCopy());
		VectorPrimitiveLayer* pSpawnedLayer = new VectorPrimitiveLayer;

		pSpawnedLayer->CopyChildrenFromLayer(pLoadedInLayer);
		Box2f spawnBox = pSpawnArea->GetBoundingBox();
		Box2f loadedInBox = pLoadedInLayer->GetBoundingBox();

		// Set up layout of new layer based off layout parameters
		switch (spawnParams.spawnDirection)
		{
		case EUIObjectSubObjectSpawnerSpawnDirection::Horizontal:
		{
			switch (spawnParams.spawnLayout)
			{
			case EUIObjectSubObjectSpawnerSpawnLayout::Center:
			{
				// Horizontal + Center
				const float xSpawnLocation = (((spawnBox.width + loadedInBox.width) / (spawnParams.numObjects + 1)) * (spawnedSubObjectIndex + 1)) - loadedInBox.width / 2.0f;
				const float ySpawnLocation = spawnBox.height / 2.0f;

				const float xIconOffset = -loadedInBox.width / 2.0f;
				const float yIconOffset = -loadedInBox.height / 2.0f;
				
				pSpawnedLayer->SetPositionOffset({xSpawnLocation + xIconOffset, ySpawnLocation + yIconOffset});
				
				break;
			}
			case EUIObjectSubObjectSpawnerSpawnLayout::Top:
			{
				DOMLOG_ERROR("Not implemented yet");
				break;
			}
			case EUIObjectSubObjectSpawnerSpawnLayout::Bottom:
			{
				DOMLOG_ERROR("Not implemented yet");
				break;
			}
			default:
			{
				DOMLOG_ERROR("Invalid direciton/layout combo")
				break;
			}
			}
			
			break;
		}
		case EUIObjectSubObjectSpawnerSpawnDirection::Vertical:
		{
			switch (spawnParams.spawnLayout)
			{
			case EUIObjectSubObjectSpawnerSpawnLayout::Center:
			{
				DOMLOG_ERROR("Not implemented yet");
				break;
			}
			case EUIObjectSubObjectSpawnerSpawnLayout::Left:
			{
				DOMLOG_ERROR("Not implemented yet");
				break;
			}
			case EUIObjectSubObjectSpawnerSpawnLayout::Right:
			{
				DOMLOG_ERROR("Not implemented yet");
				break;
			}
			default:
			{
				DOMLOG_ERROR("Invalid direciton/layout combo")
				break;
			}
			}
			
			break;
		}
		default:
		{
			DOMLOG_ERROR("Invalid spawn direction");
			break;
		}
		}
		
		pSpawnArea->AddChild(pSpawnedLayer);
	}

	return true;
}
