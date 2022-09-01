#pragma once

#include "SystemBase.h"

struct ComponentMesh;
struct ComponentTransform;

/** SystemRender
 * 
 * System to render an entity with a MeshComponent
 */
class SystemRender : public SystemBase
{
public:
	static float GRID_SIZE;

	void Tick(const SystemTickParams& params, const std::tuple<ComponentMesh*, ComponentTransform*>& components);
	void OnEntityDeleted(const SystemEntityDeletionParams& params, const std::tuple<ComponentMesh*, ComponentTransform*>& components);

protected:
	//~ Begin SystemBase Interface
	void InitialiseInternal(ECS* pEcs) override;
	//~ End SystemBase Interface
};
