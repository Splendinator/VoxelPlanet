#include "Chunk.h"

class ECS;

class ChunkGeneratorBase
{
public:
	EntityId CreateEntity(ECS* pEcs, EWorldGenerationLayer layer, int x, int y)
	{
		switch (layer)
		{
		case EWorldGenerationLayer::Foreground:
		{
			return GetForeground(pEcs, x, y);
		}
		case EWorldGenerationLayer::Background:
		{
			return GetBackground(pEcs, x, y);
		}
		default:
		{
			DOMLOG_WARN("Added new layer with no handling?");
			return ECS::INVALID_ENTITY_ID;
		}
		}
	};

protected:

	virtual EntityId GetForeground(ECS* pEcs, int x, int y) { return ECS::INVALID_ENTITY_ID; };
	virtual EntityId GetBackground(ECS* pEcs, int x, int y) { return ECS::INVALID_ENTITY_ID; };
};
