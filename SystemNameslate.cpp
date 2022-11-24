#include "pch.h"

#include "SystemNameslate.h"

#include "Components.h"
#include "FilePaths.h"
#include "RendererObject.h"
#include "SystemCallback.h"
#include "SystemRender.h"
#include "UIObjectProgressBar.h"
#include "VectorPrimitiveRectangle.h"

// HealthBarNameslate
void HealthBarNameslate::Init()
{
	const float healthBarWidth = SystemRender::GRID_SIZE * 0.8f;
	const float healthBarHeight = healthBarWidth * 0.08f;

	UICanvasInitParams initParams;
	initParams.sizeX = healthBarWidth;
	initParams.sizeY = healthBarHeight;
	initParams.filePath = FilePath::VectorArt::healthBar;
	initParams.renderPriority = RenderPriority::UI;
	initParams.type = dmgf::ERenderObjectType::InGame;

	canvas = std::make_unique<UICanvas>(initParams);

	pHealthBarProgress = canvas->GetUIObject<UIObjectProgressBar>();
}

void HealthBarNameslate::Uninit()
{
	canvas->Uninit();
	canvas.release();

	pHealthBarProgress = nullptr;
}

void HealthBarNameslate::SetPos(int x, int y)
{
	DOMASSERT(canvas, "Not initialised");

	const float XOffset = SystemRender::GRID_SIZE * 0.1f;
	const float YOffset = SystemRender::GRID_SIZE * 0.1f;

	canvas->SetPosition(XOffset + SystemRender::GRID_SIZE * x, YOffset + SystemRender::GRID_SIZE * y);
}

void HealthBarNameslate::SetHealth(int health, int maxHealth)
{
	DOMASSERT(pHealthBarProgress, "Not initialised");

	const float healthFrac = (float)health / (float)maxHealth;
	
	pHealthBarProgress->SetProgressFrac(healthFrac);
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