#include "pch.h"

#include "ECS/Components.h"
#include "ECS/ECS.h"
#include "ECS/Systems/Callbacks/ECSSystemCallback.h"
#include "ECSSystemRender.h"
#include "Renderer.h"
#include "RendererObject.h"

float ECSSystemRender::GRID_SIZE = 1.0f;

void ECSSystemRender::InitialiseInternal(ECS* pEcs)
{
	pEcs->RegisterSystemCallback(std::make_unique<ECSSystemCallback<ECSSystemRender, ComponentMesh, ComponentTransform>>(this));
}

void ECSSystemRender::Tick(const ECSSystemTickParams& params, const std::tuple<ComponentMesh*, ComponentTransform*>& components)
{
	RendererObject* pRendererObject = std::get<ComponentMesh*>(components)->pRendererObject;
	ComponentTransform* transform = std::get<ComponentTransform*>(components);

	pRendererObject->SetSize(GRID_SIZE, GRID_SIZE);
	pRendererObject->SetPosition(GRID_SIZE * transform->x, GRID_SIZE * transform->y);
}

void ECSSystemRender::OnEntityDeleted(const ECSSystemEntityDeletionParams& params, const std::tuple<ComponentMesh*, ComponentTransform*>& components)
{
	// Remove the mesh from the renderer
	if (RendererObject*& pRendererObject = std::get<ComponentMesh*>(components)->pRendererObject)
	{
		dmgf::RemoveObject(pRendererObject);
		pRendererObject = nullptr;
	}
}
