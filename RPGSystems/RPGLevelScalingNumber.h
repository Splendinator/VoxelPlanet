#pragma once

EDITORENUM()
enum class ELevelScalingAlgorithm : u8
{
	None,
	Linear,			// Add a flat amount per level
	Exponential,	// Multiply by a flat amount per level
};

EDITORSTRUCT()
struct RPGLevelScalingNumber
{
	EDITORBODY()
public:

	// Level starts at 0
	float CalculateNumber(u32 level) const;
	
protected:
	
	EDITORPROPERTY()
	ELevelScalingAlgorithm scalingAlgorithm = ELevelScalingAlgorithm::None;

	// Number you start with
	EDITORPROPERTY()
	float baseValue;

	// Number that is applied for each level
	EDITORPROPERTY()
	float perLevelScaleFactor;
};
