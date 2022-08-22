#include "pch.h"

#include "Components.h"
#include "ECS.h"
#include "RendererObject.h"
#include "SystemCallback.h"
#include "SystemRender.h"

float SystemRender::GRID_SIZE = 100.f;

void SystemRender::InitialiseInternal(ECS* pEcs)
{
	pEcs->RegisterSystemCallback(std::make_unique<SystemCallback<SystemRender, ComponentMesh, ComponentTransform>>(this));
}

void SystemRender::Tick(const SystemTickParams& params, const std::tuple<ComponentMesh*, ComponentTransform*>& components)
{
	RendererObject* pRendererObject = std::get<ComponentMesh*>(components)->pRendererObject;
	ComponentTransform* transform = std::get<ComponentTransform*>(components);

	pRendererObject->SetSize(GRID_SIZE, GRID_SIZE);
	pRendererObject->SetPosition(GRID_SIZE * transform->x, GRID_SIZE * transform->y);
}
