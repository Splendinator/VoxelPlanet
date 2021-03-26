#pragma once

#include "DomMath/Types.h"
#include "DomLog/DomLog.h"
#include "DomMath/Vec2.h"

constexpr int CHUNK_SIZE = 16;
constexpr int CHUNK_HEIGHT = 256;

enum class BlockType : u8
{
	None,
	Grass,
	Count,
	//DIRT,
};

// A chunk representation. Just an array of BlockTypes with pointers to neighboring chunks.
class Chunk
{
public:

	void SetBlock(int x, int y, int z, BlockType blockType)
	{
		blocks[x + y * CHUNK_SIZE + z * CHUNK_SIZE * CHUNK_HEIGHT] = blockType;
	}

	BlockType GetBlock(int x, int y, int z) const
	{
		DOMLOG_ERROR_IF(x >= CHUNK_SIZE, "x (", x, "is bigger than chunk size(", CHUNK_SIZE, ")");
		DOMLOG_ERROR_IF(y >= CHUNK_HEIGHT, "y (", y, "is bigger than chunk height(", CHUNK_HEIGHT, ")");
		DOMLOG_ERROR_IF(z >= CHUNK_SIZE, "z (", z, "is bigger than chunk size(", CHUNK_SIZE, ")");
		
		return blocks[x + y * CHUNK_SIZE + z * CHUNK_SIZE * CHUNK_HEIGHT];
	}

	BlockType& GetBlockRef(int x, int y, int z)
	{
		DOMLOG_ERROR_IF(x >= CHUNK_SIZE, "x (", x, "is bigger than chunk size(", CHUNK_SIZE, ")");
		DOMLOG_ERROR_IF(y >= CHUNK_HEIGHT, "y (", y, "is bigger than chunk height(", CHUNK_HEIGHT, ")");
		DOMLOG_ERROR_IF(z >= CHUNK_SIZE, "z (", z, "is bigger than chunk size(", CHUNK_SIZE, ")");

		return blocks[x + y * CHUNK_SIZE + z * CHUNK_SIZE * CHUNK_HEIGHT];
	}

	Vec2i pos = {9999999, 9999999};

	// Chunk pointers that are set the by the ChunkManager
	Chunk* pNorthChunk = nullptr;
	Chunk* pSouthChunk = nullptr;
	Chunk* pEastChunk = nullptr;
	Chunk* pWestChunk = nullptr;

private:

	// Blocks that are set by the generator
	BlockType blocks[CHUNK_SIZE * CHUNK_SIZE * CHUNK_HEIGHT] = {};
};

// Common utils for chunk generation stuff
namespace ChunkUtils
{

};

