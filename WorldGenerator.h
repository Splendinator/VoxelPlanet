#pragma once

#include "Core/GameSystem.h"

constexpr int WORLD_START_X = 10000;
constexpr int WORLD_START_Y = 10000;

class Chunk;
class ECS;

/** WorldGenerator
*
* This class is used to generate the open world of the game.
* It handles loading chunks in and out of memory, and generating the terrain
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
	//~ End GameSystem Interface
	
	// Set the center of the world generator, world will generator around these coordinates (this is likely tied to player's location)
	void SetCenter(int newX, int newY, bool bInit);

protected:

private:

	// Num chunks in a square around player ( Calculated as chunks from the player so NUM_CHUNKS == 2 means 2 in each direction + center means square of 5 chunks)
	static constexpr int NUM_CHUNKS = 3; 
	static constexpr int CHUNK_SQUARE_SIZE = NUM_CHUNKS * 2 + 1;

	EDITORPROPERTY()
	ECS* pEcs = nullptr;

	Chunk* pChunks[CHUNK_SQUARE_SIZE * CHUNK_SQUARE_SIZE];

	int centerX = INT_MIN;
	int centerY = INT_MIN;
};
