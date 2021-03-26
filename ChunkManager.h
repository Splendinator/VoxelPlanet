#pragma once

#include "ChunkUtils.h"

#include "DomGraphics.h"

#include <vector>



class Chunk;

constexpr int CHUNK_MAP_SIZE = 32; // The size squared of the chunk map. See chunk map for more.
constexpr int CHUNK_MAP_ARRAY_SIZE = CHUNK_MAP_SIZE * CHUNK_MAP_SIZE;

// Recieves raw generated chunks and does some final preprocessing before sending them to graphics renderer
class ChunkManager
{
public:
	
	// renderDistance - the distance in blocks (not chunks) to render. 
	void Initialise(u32 renderDistance) { renderDistanceSq = (float)(renderDistance * renderDistance); };
	void Uninitialise() {};

	// Returns whether we want to render a chunk or not and populates outChunkCoords if we do.
	// Returns false if all chunks near the player are already generated.
	// This is called by the generation thread.
	bool GetNextChunkToRender(Vec2i& outChunkCoords) const;

	void AddChunk(Chunk* chunk);

private:

	Chunk* GetChunk(int x, int y) const;
	Chunk*& GetChunkRef(int x, int y);
	bool IsChunkValid(int x, int y) const;
	bool IsChunkInRenderRange(int x, int y) const;

	std::vector<dmgf::Quad> GenerateQuadsForChunk(Chunk* chunk);

	// A map of all chunks. Chunks are placed into the map using their position modulus CHUNK_MAP_SIZE
	// e.g with CHUNK_MAP_SIZE = 128
	// Chunk(100,100) goes in ChunkMap[100,100]
	// Chunk(200,100) goes in at ChunkMap[200%128, 100] = ChunkMap[72,100]
	// New chunks replace old ones, so make sure CHUNK_MAP_SIZE is bigger than the render distance
	Chunk* chunkMap[CHUNK_MAP_ARRAY_SIZE];

	float renderDistanceSq = 0;
};

