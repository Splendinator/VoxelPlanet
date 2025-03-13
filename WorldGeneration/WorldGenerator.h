#pragma once

#include "Core/GameSystem.h"

class WorldGenerationContinent;
class WorldGenerationChunk;
class ECS;

/** WorldGenerator
*
* This class is used to generate the world of the game.
*
* World generation has two parts.
* 1. A single WorldGenerationContinent is generated at the start of the game that contains enough data to know what each tile is, and where each enemy will spawn etc.
* 2. WorldGenerationChunks are dynamically loaded in at runtime around the player to stream in the world, they know what tiles / enemies to spawn by querying the WorldGenerationContinent
*/
EDITORCLASS()
class WorldGenerator : public GameSystem
{
	EDITORBODY()
	
public:
	//~ Begin GameSystem Interface
	void Init() override;
	void Tick(float deltaTime) override;
	void UnInit() override;

#ifdef DOMIMGUI
	void DrawImGui(float deltaTime) override;
#endif
	//~ End GameSystem Interface

protected:

	// Set the center of the world generator, world will generator around these coordinates (this is likely tied to player's location)
	void SetCenter(Vec2i center, bool bInit);

private:

	// Num chunks in a square around player ( Calculated as chunks from the player so NUM_CHUNKS == 2 means 2 in each direction + center means square of 5 chunks)
	static constexpr int NUM_CHUNKS = 8; 
	static constexpr int CHUNK_SQUARE_SIZE = NUM_CHUNKS * 2 + 1;

	EDITORPROPERTY()
	ECS* pEcs = nullptr;

	EDITORPROPERTY()
	WorldGenerationContinent* pContinent = nullptr;

	WorldGenerationChunk* pChunks[CHUNK_SQUARE_SIZE * CHUNK_SQUARE_SIZE];
};
