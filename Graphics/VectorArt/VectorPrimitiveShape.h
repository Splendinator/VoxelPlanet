#pragma once
#include <DomMath/Vec3.h>

#include "VectorPrimitiveBase.h"
class VectorPrimitiveShape : public VectorPrimitiveBase
{
public:
	//~ Begin VectorPrimitiveBase Interface
	u32* Serialize(u32* pBuffer) override;
	std::istream& PopulateFromFile(std::istream& stream) override;
	//~ End VectorPrimitiveBase Interface

	// Call by parents when deep copying to get shape data copied over
	void SetupDeepCopiedShape(VectorPrimitiveShape* pDeepCopy) const;
	
	void SetColour(u8 R, u8 G, u8 B) { red = R; green = G; blue = B; }
	Vec3<u8> GetColour() const { return Vec3<u8>((u8)red, (u8)green, (u8)blue); }
	
	// #TODO: Do we need something here to get width and height and X and Y? If so all shapes need to return the top left X and Y and width and height
	// This could make it awkward for circles as they deal with center and radius?
	
private: 
	// Colour
	u32 red = 0;
	u32 green = 0;
	u32 blue = 0;
};

