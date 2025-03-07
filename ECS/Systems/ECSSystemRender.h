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

	// Set up the render object with the correct size and position to make it appear at a specific grid space
	// {0,0} default arguments in-case you just want to set the size
	static void SetupRenderObjectOnGrid(TransientPtr<RendererObject> pRendererObject, Vec2i gridCoordinates = {0,0});

	void Tick(const ECSSystemTickParams& params, const std::tuple<ComponentMesh*, ComponentTransform*>& components);
	void OnEntityDeleted(const ECSSystemEntityDeletionParams& params, const std::tuple<ComponentMesh*, ComponentTransform*>& components);

protected:
	//~ Begin ECSSystemBase Interface
	void InitialiseInternal(ECS* pEcs) override;
	//~ End ECSSystemBase Interface
};
