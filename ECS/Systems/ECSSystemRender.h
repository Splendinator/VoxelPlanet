#pragma once

#include "ECSSystemBase.h"

struct ComponentMesh;
struct ComponentTransform;

/** ECSSystemRender
 * 
 * System to render an entity with a MeshComponent
 */
EDITORCLASS()
class ECSSystemRender : public ECSSystemBase
{
	EDITORBODY()
public:
	static float GRID_SIZE;

	void Tick(const ECSSystemTickParams& params, const std::tuple<ComponentMesh*, ComponentTransform*>& components);
	void OnEntityDeleted(const ECSSystemEntityDeletionParams& params, const std::tuple<ComponentMesh*, ComponentTransform*>& components);

protected:
	//~ Begin ECSSystemBase Interface
	void InitialiseInternal(ECS* pEcs) override;
	//~ End ECSSystemBase Interface
};
