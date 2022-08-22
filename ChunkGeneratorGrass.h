#pragma once

#include "ChunkGeneratorBase.h"

class ChunkGeneratorGrass : public ChunkGeneratorBase
{
public:
	//~ Begin ChunkGeneratorBase Interface
	EntityId GetForeground(ECS* pEcs, int x, int y) override;
	EntityId GetBackground(ECS* pEcs, int x, int y) override;
	//~ End ChunkGeneratorBase Interface
};