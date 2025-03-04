#include "pch.h"

#include "RPGAttributes.h"

#include "ECS.h"
#include "RPGAttributeModifiers.h"
#include "RPGAttributesShared.h"

#include "RPGSystems/Races/RPGRaceData.h"

// RPGAttributeBase
int RPGAttributeBase::GetAttributeValue(const EntityId& entity, const RPGAttributeCalculationSharedData& sharedData) const
{
	if (!CanApplyAttribute(entity, sharedData))
	{
		DOMLOG_ERROR("Can't apply this");
		return 0;
	}
	
	DOMLOG_ERROR_IF(!sharedData.pEcs->EntityHasComponents<ComponentRace>(entity), "Without race there's no base value for attributes, if this is hit we're using uninitialised data")
	int value = GetBaseAttribute(entity, sharedData);

	for (RPGAttributeModifierBase* pModifier : pModifiers)
	{
		pModifier->Modify(value, entity, sharedData);
	}

	return value;
}

int RPGAttributeBase::CalculateInitialAdditiveAttribute(int initialValue, int additivePerLevel, int level) const
{
	return initialValue + additivePerLevel * level;
}

int RPGAttributeBase::CalculateInitialExponentialAttribute(int initialValue, float multiplierPerLevel, int level) const
{
	float cumulativeMultiplier = 1.0f;
	
	for (int i = 0; i < level; i++)
	{
		cumulativeMultiplier *= multiplierPerLevel;
	}

	return (int)((float)initialValue * cumulativeMultiplier);
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

int RPGAttributeMaxHealth::GetBaseAttribute(const EntityId& entity, const RPGAttributeCalculationSharedData& sharedData) const
{
	ComponentRace& raceComponent = sharedData.pEcs->GetComponent<ComponentRace>(entity);
	ComponentProgression& progressionComponent = sharedData.pEcs->GetComponent<ComponentProgression>(entity);
	return CalculateInitialExponentialAttribute(raceComponent.pRaceData->initialAttributes.maxHealth, raceComponent.pRaceData->perLevelAttributes.maxHealthMultiplierPerLevel, progressionComponent.level);
}
