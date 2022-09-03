#include "pch.h"

#include "SystemNameslate.h"

#include "Components.h"
#include "FilePaths.h"
#include "Renderer.h"
#include "RendererObject.h"
#include "SystemCallback.h"
#include "SystemRender.h"
#include "VectorArt.h"
#include "VectorPrimitiveRectangle.h"
#include "RenderPriorities.h"

// Width of health bar when at full health, populated when reading in file
static u32 healthBarTotalWidth = 0;

// HealthBarNameslate
void HealthBarNameslate::Init()
{
	std::shared_ptr<VectorArt> pVectorArt = std::make_shared<VectorArt>(FilePath::VectorArt::healthBar);

	pHealthBarProgress = pVectorArt->FindPrimitiveByLabel<VectorPrimitiveRectangle>("Foreground");
	pHealthBarRenderObject = dmgf::AddObjectFromVectorArt(pVectorArt);

	pHealthBarRenderObject->SetRenderPriority(RenderPriority::UI);

	const float healthBarWidth = SystemRender::GRID_SIZE * 0.8f;
	const float healthBarHeight = healthBarWidth * 0.08f;
	pHealthBarRenderObject->SetSize(healthBarWidth, healthBarHeight);

	healthBarTotalWidth = pVectorArt->GetPageWidth();
}

void HealthBarNameslate::Uninit()
{
	pHealthBarProgress = nullptr;
	dmgf::RemoveObject(pHealthBarRenderObject);
	pHealthBarRenderObject = nullptr;
}

void HealthBarNameslate::SetPos(int x, int y)
{
	DOMASSERT(IsInitialised(), "Should be initialised");

	const float XOffset = SystemRender::GRID_SIZE * 0.1f;
	const float YOffset = SystemRender::GRID_SIZE * 0.1f;

	pHealthBarRenderObject->SetPosition(XOffset + SystemRender::GRID_SIZE * x, YOffset + SystemRender::GRID_SIZE * y);
}

void HealthBarNameslate::SetHealth(int health, int maxHealth)
{
	DOMASSERT(IsInitialised(), "Should be initialised");
	
	const float healthFrac = (float)health / (float)maxHealth;
	const u32 healthBarWidth = (u32)((float)healthBarTotalWidth * healthFrac);

	pHealthBarProgress->SetWidth(healthBarWidth);
}

// SystemNameslate
void SystemNameslate::InitialiseInternal(ECS* pEcs)
{
	pEcs->RegisterSystemCallback(std::make_unique<SystemCallback<SystemNameslate, ComponentTransform, ComponentHealth>>(this));
}

void SystemNameslate::Tick(const SystemTickParams& params, const std::tuple<ComponentTransform*, ComponentHealth*>& components)
{
	ComponentHealth* pHealth = std::get<ComponentHealth*>(components);
	ComponentTransform* pTransform = std::get<ComponentTransform*>(components);

	HealthBarNameslate& nameslate = nameslates[params.entityId];
	if (!nameslate.IsInitialised())
	{
		// #TODO: Move this to OnEntityCreated instead of checking on Tick()
		nameslate.Init();
	}
	
	nameslate.SetPos(pTransform->x, pTransform->y);
	nameslate.SetHealth(pHealth->health, pHealth->maxHealth);
}

void SystemNameslate::OnEntityDeleted(const SystemEntityDeletionParams& params, const std::tuple<ComponentTransform*, ComponentHealth*>& components)
{
	HealthBarNameslate& nameslate = nameslates[params.entityId];
	nameslate.Uninit();
}