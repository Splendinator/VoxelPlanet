#pragma once

#include "ECSTypes.h"

struct RPGAttributeCalculationSharedData;

EDITORCLASS(Abstract, EditInlineNew)
class RPGAttributeModifierBase
{
	EDITORBODY()
public:

	virtual void Modify(int& inOutValue, const EntityId& entity, const RPGAttributeCalculationSharedData& sharedData) { PUREVIRTUAL() }
};

// #TEMP: Remove
EDITORCLASS()
class RPGAttributeModifierTest : public RPGAttributeModifierBase
{
	EDITORBODY()
public:
	//~ Begin RPGAttributeModifierBase Interface
	void Modify(int& inOutValue, const EntityId& entity, const RPGAttributeCalculationSharedData& sharedData) override { inOutValue += delta; }
	//~ End RPGAttributeModifierBase Interface

	EDITORPROPERTY()
	int delta = 0;
};