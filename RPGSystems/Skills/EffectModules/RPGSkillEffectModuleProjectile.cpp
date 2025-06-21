#include "pch.h"

#include "RPGSkillEffectModuleProjectile.h"

#include "AI/Actions/ActionDeciders/ActionDeciderProjectile.h"
#include "Core/DirectoryData.h"
#include "ECS/ECS.h"
#include "ECS/Systems/ECSSystemRender.h"

#include "Graphics/Renderer.h"
#include "Graphics/RendererObject.h"
#include "Graphics/RenderPriorities.h"

void RPGSkillEffectModuleProjectile::ApplyEffect(const RPGSkillParams& params) const
{
	EntityId projectileEntity = params.ecs.GetNextFreeEntity();

	ComponentTransform& transformComponent = params.ecs.AddComponent<ComponentTransform>(projectileEntity);
	transformComponent.x = params.startPos.x;
	transformComponent.y = params.startPos.y;

	ComponentProjectile& projectileComponent = params.ecs.AddComponent<ComponentProjectile>(projectileEntity);
	projectileComponent.pProjectileEffect = pProjectileEffect;
	projectileComponent.startX = params.startPos.x;
	projectileComponent.startY = params.startPos.y;
	projectileComponent.targetX = params.targetPos.x;
	projectileComponent.targetY = params.targetPos.y;
	projectileComponent.caster = params.caster;
	projectileComponent.speed = speed;

	ComponentAction& actionComponent = params.ecs.AddComponent<ComponentAction>(projectileEntity);
	actionComponent.pActionDecider = pActionDecider;
	actionComponent.energy = 0;
	actionComponent.maxEnergy = 1;
	
	TransientPtr<RendererObject> pRenderObject = dmgf::AddObjectFromSVG(DirectoryData::ConcatenateSVGFilePathChecked(params.directoryData.rpgSkillVisuals, projectileFileName).c_str());
	ECSSystemRender::SetupRenderObjectOnGrid(pRenderObject, {params.startPos.x, params.startPos.y});
	pRenderObject->SetRenderPriority(RenderPriority::unit);

	ComponentMesh& meshComponent = params.ecs.AddComponent<ComponentMesh>(projectileEntity);
	meshComponent.pRendererObject = pRenderObject;
}
