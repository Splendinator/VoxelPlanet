#pragma once

#include "DomMath/Types.h"

enum class EPrimitiveType : u32
{
	END = 0, // Reserved -- End of primitive stream
	PushLayer = 1, // Layer consisting of many children primitives
	PopLayer = 2, // End of layer
	Cirlce = 3, // Circle
	Rectangle = 4, // Rectangle
};