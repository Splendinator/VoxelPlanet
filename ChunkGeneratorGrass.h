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

/// #TEMP: remove
class ChunkGeneratorDebug : public ChunkGeneratorGrass
{
public:
	//~ Begin ChunkGeneratorBase Interface
	EntityId GetBackground(ECS* pEcs, int x, int y) override;
	//~ End ChunkGeneratorBase Interface
};