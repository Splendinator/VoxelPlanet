#pragma once

#include "ECS.h"

class Chunk;

/** WorldGenerator
*
* This class is used to generate the open world of the game.
* It handles loading chunks in and out of memory, and generating the terrain
*/
class WorldGenerator
{
public:
	
	WorldGenerator(ECS& inEcs) : ecs(inEcs) {}

	// Set the center of the world generator, world will generator around these coordinates (this is likely tied to player's location)
	void SetCenter(int newX, int newY, bool bInit);
	void Uninitialise();

protected:

private:

	// Num chunks in a square around player ( Calculated as chunks from the player so NUM_CHUNKS == 2 means 2 in each direction + center means square of 5 chunks)
	static constexpr int NUM_CHUNKS = 3; 
	static constexpr int CHUNK_SQUARE_SIZE = NUM_CHUNKS * 2 + 1;

	ECS& ecs;

	Chunk* pChunks[CHUNK_SQUARE_SIZE * CHUNK_SQUARE_SIZE];

	int centerX = INT_MIN;
	int centerY = INT_MIN;
};
