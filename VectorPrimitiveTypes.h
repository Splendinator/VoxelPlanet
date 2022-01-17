#pragma once

#include "DomMath/Types.h"

enum class EPrimitiveType : u32
{
	END = 0, // Reserved -- End of primitive stream
	Layer = 1, // Layer consisting of many children primitives
	Cirlce = 2, // Circle
	Rectangle = 3, // Rectangle
};