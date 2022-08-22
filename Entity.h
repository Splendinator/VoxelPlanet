#pragma once

#include "Components.h"
#include "DomMath/Types.h"

class Components
{
public:

	bool HasComponent(EComponents component) const { return components & GetBitValue(component); }
	bool HasAnyComponents() const { return components != 0; }
	void AddComponent(EComponents component) { components |= GetBitValue(component); }
	void RemoveComponent(EComponents component) { components &= ~GetBitValue(component); }
	void RemoveAllComponents() { components = 0; }

private:

	u64 GetBitValue(EComponents component) const { return (1ull << (u64)component); }

	u64 components; // Bitfield of all components
};

class Entity
{
public:
	Components components;
};