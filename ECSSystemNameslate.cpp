#include "pch.h"

#include "ECSSystemNameslate.h"

#include "Components.h"
#include "FilePaths.h"
#include "RendererObject.h"
#include "ECSSystemCallback.h"
#include "ECSSystemRender.h"
#include "UI/UIObjects/UIObjectProgressBar.h"
#include "VectorPrimitiveRectangle.h"

// HealthBarNameslate
void HealthBarNameslate::Init()
{
	const float healthBarWidth = ECSSystemRender::GRID_SIZE * 0.8f;
	const float healthBarHeight = healthBarWidth * 0.08f;

	UICanvasInitParams initParams;
	initParams.sizeX = healthBarWidth;
	initParams.sizeY = healthBarHeight;
	initParams.filePath = FilePath::VectorArt::healthBar;
	initParams.renderPriority = RenderPriority::inGameUI;
	initParams.type = dmgf::ERenderObjectType::InGame;

	canvas = std::make_unique<UICanvas>(initParams);

	pHealthBarProgress = canvas->GetUIObject<UIObjectProgressBar>();
}

void HealthBarNameslate::Uninit()
{
	canvas->UnInit();
	canvas.release();

	pHealthBarProgress = nullptr;
}

void HealthBarNameslate::SetPos(int x, int y)
{
	DOMASSERT(canvas, "Not initialised");

	const float XOffset = ECSSystemRender::GRID_SIZE * 0.1f;
	const float YOffset = ECSSystemRender::GRID_SIZE * 0.1f;

	canvas->SetPosition(XOffset + ECSSystemRender::GRID_SIZE * x, YOffset + ECSSystemRender::GRID_SIZE * y);
}

void HealthBarNameslate::SetHealth(int health, int maxHealth)
{
	DOMASSERT(pHealthBarProgress, "Not initialised");

	const float healthFrac = (float)health / (float)maxHealth;
	
	pHealthBarProgress->SetProgressFrac(healthFrac);
}

// ECSSystemNameslate
void ECSSystemNameslate::InitialiseInternal(ECS* pEcs)
{
	pEcs->RegisterSystemCallback(std::make_unique<ECSSystemCallback<ECSSystemNameslate, ComponentTransform, ComponentHealth>>(this));
}

void ECSSystemNameslate::Tick(const ECSSystemTickParams& params, const std::tuple<ComponentTransform*, ComponentHealth*>& components)
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

void ECSSystemNameslate::OnEntityDeleted(const ECSSystemEntityDeletionParams& params, const std::tuple<ComponentTransform*, ComponentHealth*>& components)
{
	HealthBarNameslate& nameslate = nameslates[params.entityId];
	nameslate.Uninit();
}