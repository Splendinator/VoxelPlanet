#include "ChunkManager.h"

#include "DomGraphics.h"
#include "../Graphics/Camera.h"

namespace
{
	enum EQuadDir
	{
		POSY,
		NEGY,
		POSX,
		NEGX,
		POSZ,
		NEGZ
	};

	dmgf::Quad GenerateQuad(const Vec3f& position, EQuadDir dir, const Vec3f &colour)
	{
		switch (dir)
		{
		case EQuadDir::POSZ:
		{
			const Vec3f pos1 = position + Vec3f(-0.5f, -0.5f, 0.5f);
			const Vec3f pos2 = position + Vec3f(-0.5f, 0.5f, 0.5f);
			const Vec3f pos3 = position + Vec3f(0.5f, 0.5f, 0.5f);
			const Vec3f pos4 = position + Vec3f(-0.5f, -0.5f, 0.5f);
			const Vec3f pos5 = position + Vec3f(0.5f, 0.5f, 0.5f);
			const Vec3f pos6 = position + Vec3f(0.5f, -0.5f, 0.5f);
			const Vec3f normal(0.0f, 0.0f, 1.0f);

			return
			{
				pos1.x, pos1.y, pos1.z, normal.x, normal.y, normal.z, colour.x, colour.y, colour.z,
				pos2.x, pos2.y, pos2.z, normal.x, normal.y, normal.z, colour.x, colour.y, colour.z,
				pos3.x, pos3.y, pos3.z, normal.x, normal.y, normal.z, colour.x, colour.y, colour.z,
				pos4.x, pos4.y, pos4.z, normal.x, normal.y, normal.z, colour.x, colour.y, colour.z,
				pos5.x, pos5.y, pos5.z, normal.x, normal.y, normal.z, colour.x, colour.y, colour.z,
				pos6.x, pos6.y, pos6.z, normal.x, normal.y, normal.z, colour.x, colour.y, colour.z
			};
		}
		case EQuadDir::NEGZ:
		{
			const Vec3f pos1 = position + Vec3f(-0.5f, -0.5f, -0.5f);
			const Vec3f pos2 = position + Vec3f(-0.5f, 0.5f, -0.5f);
			const Vec3f pos3 = position + Vec3f(0.5f, 0.5f, -0.5f);
			const Vec3f pos4 = position + Vec3f(-0.5f, -0.5f, -0.5f);
			const Vec3f pos5 = position + Vec3f(0.5f, 0.5f, -0.5f);
			const Vec3f pos6 = position + Vec3f(0.5f, -0.5f, -0.5f);
			const Vec3f normal(0.0f, 0.0f, -1.0f);

			return
			{
				pos1.x, pos1.y, pos1.z, normal.x, normal.y, normal.z, colour.x, colour.y, colour.z,
				pos2.x, pos2.y, pos2.z, normal.x, normal.y, normal.z, colour.x, colour.y, colour.z,
				pos3.x, pos3.y, pos3.z, normal.x, normal.y, normal.z, colour.x, colour.y, colour.z,
				pos4.x, pos4.y, pos4.z, normal.x, normal.y, normal.z, colour.x, colour.y, colour.z,
				pos5.x, pos5.y, pos5.z, normal.x, normal.y, normal.z, colour.x, colour.y, colour.z,
				pos6.x, pos6.y, pos6.z, normal.x, normal.y, normal.z, colour.x, colour.y, colour.z
			};
		}
		case EQuadDir::POSX:
		{
			const Vec3f pos1 = position + Vec3f(0.5f, -0.5f, -0.5f);
			const Vec3f pos2 = position + Vec3f(0.5f, 0.5f, -0.5f);
			const Vec3f pos3 = position + Vec3f(0.5f, 0.5f, 0.5f);
			const Vec3f pos4 = position + Vec3f(0.5f, -0.5f, -0.5f);
			const Vec3f pos5 = position + Vec3f(0.5f, 0.5f, 0.5f);
			const Vec3f pos6 = position + Vec3f(0.5f, -0.5f, 0.5f);
			const Vec3f normal(1.0f, 0.0f, 0.0f);

			return
			{
				pos1.x, pos1.y, pos1.z, normal.x, normal.y, normal.z, colour.x, colour.y, colour.z,
				pos2.x, pos2.y, pos2.z, normal.x, normal.y, normal.z, colour.x, colour.y, colour.z,
				pos3.x, pos3.y, pos3.z, normal.x, normal.y, normal.z, colour.x, colour.y, colour.z,
				pos4.x, pos4.y, pos4.z, normal.x, normal.y, normal.z, colour.x, colour.y, colour.z,
				pos5.x, pos5.y, pos5.z, normal.x, normal.y, normal.z, colour.x, colour.y, colour.z,
				pos6.x, pos6.y, pos6.z, normal.x, normal.y, normal.z, colour.x, colour.y, colour.z
			};
		}
		case EQuadDir::NEGX:
		{
			const Vec3f pos1 = position + Vec3f(-0.5f, -0.5f, -0.5f);
			const Vec3f pos2 = position + Vec3f(-0.5f, 0.5f, -0.5f);
			const Vec3f pos3 = position + Vec3f(-0.5f, 0.5f, 0.5f);
			const Vec3f pos4 = position + Vec3f(-0.5f, -0.5f, -0.5f);
			const Vec3f pos5 = position + Vec3f(-0.5f, 0.5f, 0.5f);
			const Vec3f pos6 = position + Vec3f(-0.5f, -0.5f, 0.5f);
			const Vec3f normal(-1.0f, 0.0f, 0.0f);

			return
			{
				pos1.x, pos1.y, pos1.z, normal.x, normal.y, normal.z, colour.x, colour.y, colour.z,
				pos2.x, pos2.y, pos2.z, normal.x, normal.y, normal.z, colour.x, colour.y, colour.z,
				pos3.x, pos3.y, pos3.z, normal.x, normal.y, normal.z, colour.x, colour.y, colour.z,
				pos4.x, pos4.y, pos4.z, normal.x, normal.y, normal.z, colour.x, colour.y, colour.z,
				pos5.x, pos5.y, pos5.z, normal.x, normal.y, normal.z, colour.x, colour.y, colour.z,
				pos6.x, pos6.y, pos6.z, normal.x, normal.y, normal.z, colour.x, colour.y, colour.z
			};
		}
		case EQuadDir::POSY:
		{
			const Vec3f pos1 = position + Vec3f(-0.5f, 0.5f, -0.5f);
			const Vec3f pos2 = position + Vec3f(-0.5f, 0.5f, 0.5f);
			const Vec3f pos3 = position + Vec3f(0.5f, 0.5f, 0.5f);
			const Vec3f pos4 = position + Vec3f(-0.5f, 0.5f, -0.5f);
			const Vec3f pos5 = position + Vec3f(0.5f, 0.5f, 0.5f);
			const Vec3f pos6 = position + Vec3f(0.5f, 0.5f, -0.5f);
			const Vec3f normal(0.0f, 1.0f, 0.0f);

			return
			{
				pos1.x, pos1.y, pos1.z, normal.x, normal.y, normal.z, colour.x, colour.y, colour.z,
				pos2.x, pos2.y, pos2.z, normal.x, normal.y, normal.z, colour.x, colour.y, colour.z,
				pos3.x, pos3.y, pos3.z, normal.x, normal.y, normal.z, colour.x, colour.y, colour.z,
				pos4.x, pos4.y, pos4.z, normal.x, normal.y, normal.z, colour.x, colour.y, colour.z,
				pos5.x, pos5.y, pos5.z, normal.x, normal.y, normal.z, colour.x, colour.y, colour.z,
				pos6.x, pos6.y, pos6.z, normal.x, normal.y, normal.z, colour.x, colour.y, colour.z
			};
		}
		case EQuadDir::NEGY:
		{
			const Vec3f pos1 = position + Vec3f(-0.5f, -0.5f, -0.5f);
			const Vec3f pos2 = position + Vec3f(-0.5f, -0.5f, 0.5f);
			const Vec3f pos3 = position + Vec3f(0.5f, -0.5f, 0.5f);
			const Vec3f pos4 = position + Vec3f(-0.5f, -0.5f, -0.5f);
			const Vec3f pos5 = position + Vec3f(0.5f, -0.5f, 0.5f);
			const Vec3f pos6 = position + Vec3f(0.5f, -0.5f, -0.5f);
			const Vec3f normal = Vec3f(0.0f, -1.0f, 0.0f);

			return
			{
				pos1.x, pos1.y, pos1.z, normal.x, normal.y, normal.z, colour.x, colour.y, colour.z,
				pos2.x, pos2.y, pos2.z, normal.x, normal.y, normal.z, colour.x, colour.y, colour.z,
				pos3.x, pos3.y, pos3.z, normal.x, normal.y, normal.z, colour.x, colour.y, colour.z,
				pos4.x, pos4.y, pos4.z, normal.x, normal.y, normal.z, colour.x, colour.y, colour.z,
				pos5.x, pos5.y, pos5.z, normal.x, normal.y, normal.z, colour.x, colour.y, colour.z,
				pos6.x, pos6.y, pos6.z, normal.x, normal.y, normal.z, colour.x, colour.y, colour.z
			};
		}
		}

		DOMLOG_WARN("Unsupported quad direction", dir);
		return {};
	}
}

std::vector<dmgf::Quad> ChunkManager::GenerateQuadsForChunk(Chunk* chunk)
{
	std::vector<dmgf::Quad> generatedQuads;

	Vec3f colourMap[(int)BlockType::Count];
	colourMap[(int)BlockType::Grass] = { 0, 0.5f, 0.0f };

	for (int x = 0; x < CHUNK_SIZE; ++x)
	{
		for (int y = 0; y < CHUNK_HEIGHT; ++y)
		{
			for (int z = 0; z < CHUNK_SIZE; ++z)
			{
				BlockType blockType = chunk->GetBlock(x, y, z);
				const Vec3f chunkPos = { (float)chunk->pos.x * CHUNK_SIZE, 0.f, (float)chunk->pos.y* CHUNK_SIZE };
				const Vec3f blockPos = { (float)x, (float)y, (float)z };
				if (blockType != BlockType::None)
				{
					if (x > 0 && !((bool)chunk->GetBlock(x-1, y, z)))
					{
						generatedQuads.push_back(GenerateQuad(chunkPos + blockPos, EQuadDir::NEGX, colourMap[(int)blockType]));
					}
					if (x < CHUNK_SIZE - 1 && !((bool)chunk->GetBlock(x + 1, y, z)))
					{
						generatedQuads.push_back(GenerateQuad(chunkPos + blockPos, EQuadDir::POSX, colourMap[(int)blockType]));
					}
					if (y > 0 && !((bool)chunk->GetBlock(x, y - 1, z)))
					{
						generatedQuads.push_back(GenerateQuad(chunkPos + blockPos, EQuadDir::NEGY, colourMap[(int)blockType]));
					}
					if (y < CHUNK_HEIGHT - 1 && !((bool)chunk->GetBlock(x, y + 1, z)))
					{
						generatedQuads.push_back(GenerateQuad(chunkPos + blockPos, EQuadDir::POSY, colourMap[(int)blockType]));
					}
					if (z > 0 && !((bool)chunk->GetBlock(x, y, z - 1)))
					{
						generatedQuads.push_back(GenerateQuad(chunkPos + blockPos, EQuadDir::NEGZ, colourMap[(int)blockType]));
					}
					if (z < CHUNK_SIZE - 1 && !((bool)chunk->GetBlock(x, y, z + 1)))
					{
						generatedQuads.push_back(GenerateQuad(chunkPos + blockPos, EQuadDir::POSZ, colourMap[(int)blockType]));
					}
				}
			}
		}
	}

	return generatedQuads;
}

bool ChunkManager::GetNextChunkToRender(Vec2i& outChunkCoords) const
{
	///TODO: Spiral out from the player camera center
	
	constexpr int HALF_MAP_SIZE = CHUNK_MAP_SIZE / 2;
	const Vec3f cameraPos = dmgf::GetCamera().GetPos();
	const float cameraPosX = (cameraPos.x / CHUNK_SIZE);
	const float cameraPosY = (cameraPos.z / CHUNK_SIZE);

	const int negX = (int)(cameraPosX < 0.0f);
	const int negY = (int)(cameraPosY < 0.0f);

	const int baseXOffset = (int)(cameraPosX / (float)CHUNK_MAP_SIZE) - negX;
	const int baseYOffset = (int)(cameraPosY / (float)CHUNK_MAP_SIZE) - negY;
	const int cameraGridPosX = negX * CHUNK_MAP_SIZE + (int)(cameraPosX) % CHUNK_MAP_SIZE;
	const int cameraGridPosY = negY * CHUNK_MAP_SIZE + (int)(cameraPosY) % CHUNK_MAP_SIZE;

	for (int x = 0; x < CHUNK_MAP_SIZE; ++x)
	{
		const int xDiff = cameraGridPosX - x;
		int xOffset = baseXOffset;
		if (xDiff > HALF_MAP_SIZE)
		{
			++xOffset;
		}
		else if (xDiff < -HALF_MAP_SIZE)
		{
			--xOffset;
		}
		const int finalXValue = xOffset >= 0 ? xOffset * CHUNK_MAP_SIZE + x : (xOffset * CHUNK_MAP_SIZE + x);

		for (int y = 0; y < CHUNK_MAP_SIZE; ++y)
		{
			const int yDiff = cameraGridPosY - y;
			int yOffset = baseYOffset;
			if (yDiff > HALF_MAP_SIZE)
			{
				++yOffset;
			}
			else if (yDiff < -HALF_MAP_SIZE)
			{
				--yOffset;
			}
			const int finalYValue = yOffset * CHUNK_MAP_SIZE + y;

			if (IsChunkInRenderRange(finalXValue, finalYValue) && !IsChunkValid(x, y))
			{
				outChunkCoords = Vec2i({ finalXValue, finalYValue });
				return true;
			}
		}
	}

	return false;
}

void ChunkManager::AddChunk(Chunk* chunk)
{
	int x = chunk->pos.x % CHUNK_MAP_SIZE;
	int y = chunk->pos.y % CHUNK_MAP_SIZE;

	if (x < 0) x += CHUNK_MAP_SIZE;
	if (y < 0) y += CHUNK_MAP_SIZE;

	DOMLOG_WARN_IF(IsChunkValid(x,y), "Replacing valid chunk!");

	Chunk*& oldChunk = GetChunkRef(x, y);
	if (oldChunk)
	{
		dmgf::RemoveChunk(oldChunk);
		delete oldChunk;
	}
	oldChunk = chunk;

	std::vector<dmgf::Quad> chunkQuads = GenerateQuadsForChunk(chunk);

	dmgf::AddChunk(chunk, chunkQuads);
}

Chunk* ChunkManager::GetChunk(int x, int y) const
{
	DOMLOG_ERROR_IF(x >= CHUNK_MAP_SIZE || x < 0, "X coordinate wrong!", x);
	DOMLOG_ERROR_IF(y >= CHUNK_MAP_SIZE || y < 0, "Y coordinate wrong!", y);

	return chunkMap[x + y * CHUNK_MAP_SIZE];
}

Chunk*& ChunkManager::GetChunkRef(int x, int y)
{
	DOMLOG_ERROR_IF(x >= CHUNK_MAP_SIZE || x < 0, "X coordinate wrong!", x);
	DOMLOG_ERROR_IF(y >= CHUNK_MAP_SIZE || y < 0, "Y coordinate wrong!", y);

	return chunkMap[x + y * CHUNK_MAP_SIZE];
}

bool ChunkManager::IsChunkValid(int x, int y) const
{
	if (Chunk* chunk = GetChunk(x, y))
	{
		return IsChunkInRenderRange(chunk->pos.x, chunk->pos.y);
	}

	return false;
}

bool ChunkManager::IsChunkInRenderRange(int x, int y) const
{
	const Camera& playerCamera = dmgf::GetCamera();
	const Vec3f& cameraPos = playerCamera.GetPos();
	const Vec2i distanceVec = Vec2i((int)cameraPos.x, (int)cameraPos.z) - Vec2i(x * CHUNK_SIZE, y * CHUNK_SIZE);

	return (distanceVec.x * distanceVec.x) + (distanceVec.y * distanceVec.y) < renderDistanceSq;
}
