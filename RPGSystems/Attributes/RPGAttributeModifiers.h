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
