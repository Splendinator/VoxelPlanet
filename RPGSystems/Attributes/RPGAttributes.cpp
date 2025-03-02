#include "pch.h"

#include "RPGAttributes.h"

#include "ECS.h"
#include "RPGAttributeModifiers.h"
#include "RPGAttributesShared.h"

// RPGAttributeBase
int RPGAttributeBase::GetAttributeValue(const EntityId& entity, const RPGAttributeCalculationSharedData& sharedData) const
{
	if (!CanApplyAttribute(entity, sharedData))
	{
		DOMLOG_ERROR("Can't apply this");
		return baseValue;
	}
	
	int value = baseValue;

	for (RPGAttributeModifierBase* pModifier : pModifiers)
	{
		pModifier->Modify(value, entity, sharedData);
	}

	return value;
}

// RPGAttributeMaxHealth
bool RPGAttributeMaxHealth::CanApplyAttribute(const EntityId& entity, const RPGAttributeCalculationSharedData& sharedData) const
{
	return sharedData.pEcs->EntityHasComponents<ComponentHealth>(entity);
}

void RPGAttributeMaxHealth::ApplyAttribute(EntityId& entity, const RPGAttributeCalculationSharedData& sharedData) const
{
	sharedData.pEcs->GetComponent<ComponentHealth>(entity).maxHealth = GetAttributeValue(entity, sharedData);
}
