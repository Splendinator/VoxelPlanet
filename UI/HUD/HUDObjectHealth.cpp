#include "pch.h"

#include "HUDObjectHealth.h"

#include "Components.h"
#include "ECS.h"
#include "UI/UIObjects/UIObjectProgressBar.h"

void HUDObjectHealth::Init(UICanvas& canvas)
{
	pHealthBarProgress = canvas.AddExistingUIObject<UIObjectProgressBar>();
}

void HUDObjectHealth::Tick(ECS& ecs, EntityId player, float deltaTime)
{
	ComponentHealth& healthComponent = ecs.GetComponent<ComponentHealth>(player);
	float health = (float)healthComponent.health;
	float maxHealth = (float)healthComponent.maxHealth;

	pHealthBarProgress->SetProgressFrac(health / maxHealth);
}
