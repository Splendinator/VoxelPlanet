#pragma once

#include "ECSTypes.h"

class ECS;
class RPGAttributeModifierBase;
struct RPGAttributeCalculationSharedData;

// In this game RPG attributes are any deteministic numbers that are modified by things like gear, level, class, other attributes, etc.
// e.g. MaxHealth, Strength, ManaRegen, etc. but not CurrentHealth as that will just be a per-entity value on the HealthComponent
// Attributes are calculated with a base value that has a vector of modifiers applied one at a time. (MaxHealth(base=100) -> ModifierStrength(+3 per strength) -> ModifierClass (-3 per level if mage)
// Once calculated they can be apply themselves to an ECS entity
// #NOTE: We need to decide what to do if we ever have transient bonuses like "+4 strength while near 3 or more enemies" on gear or spells etc.
EDITORCLASS(Abstract, Singleton)
class RPGAttributeBase
{
	EDITORBODY()

public:
	
	virtual bool CanApplyAttribute(const EntityId& entity, const RPGAttributeCalculationSharedData& sharedData) const { PUREVIRTUAL() return false; }
	virtual void ApplyAttribute(EntityId& entity, const RPGAttributeCalculationSharedData& sharedData) const { PUREVIRTUAL() }
	virtual int GetBaseAttribute(const EntityId& entity, const RPGAttributeCalculationSharedData& sharedData) const { PUREVIRTUAL() return 0; }

	int GetAttributeValue(const EntityId& entity, const RPGAttributeCalculationSharedData& sharedData) const;

	// Player facing name of this attribute
	EDITORPROPERTY()
	std::string displayName;
	
protected:

	// Used for attributes that increase linearly per level
	int CalculateInitialAdditiveAttribute(int initialValue, int additivePerLevel, int level) const;

	// Used for attributes that increase multiplicatively per level (10% more health per level)
	int CalculateInitialExponentialAttribute(int initialValue, float multiplierPerLevel, int level) const;
	
	// List of modifiers executed sequentially, so order matters. (100 + 10 * 1.1 != 100 * 1.1 + 10)
	EDITORPROPERTY()
	std::vector<RPGAttributeModifierBase*> pModifiers;
};

EDITORCLASS()
class RPGAttributeMaxHealth : public RPGAttributeBase
{
	EDITORBODY()

	//~ Begin RPGAttributeMaxHealth Interface
	bool CanApplyAttribute(const EntityId& entity, const RPGAttributeCalculationSharedData& sharedData) const override;
	void ApplyAttribute(EntityId& entity, const RPGAttributeCalculationSharedData& sharedData) const override;
	int GetBaseAttribute(const EntityId& entity, const RPGAttributeCalculationSharedData& sharedData) const override;
	//~ End RPGAttributeMaxHealth Interface
};
