#pragma once

#include "WorldGenerationUtils.h"

// Data driven way to combine world generation logic operations using EditInlineNew logic blocks.
EDITORCLASS(Abstract, Instanced, EditInlineNew)
class WorldGenerationLogicBase
{
	EDITORBODY()
public:

	float BaseDoLogic(RandSeed& inOutSeed, float x, float y);
	
protected:
	
	// Override this to do some logic to X and Y. Some operations are 1D and will only read/use the X value.
	virtual float DoLogic(RandSeed& inOutSeed, float x, float y) const { PUREVIRTUAL() return 0.0f;}

	// Whether to deteministically mutate the seed after this logic unit has ran.
	// This will have the effect of making further logic units in this chain have a totally different seed 
	EDITORPROPERTY()
	bool bMutateSeedAfterLogic = false;
};

// Add the results of multiple other logic units 
EDITORCLASS()
class WorldGenerationLogicAdd : public WorldGenerationLogicBase
{
	EDITORBODY()
public:
	//~ Begin WorldGenerationLogicBase Interface
	float DoLogic(RandSeed& inOutSeed, float x, float y) const override;
	//~ End WorldGenerationLogicBase Interface

protected:
	// These are added in sequence order
	EDITORPROPERTY()
	std::vector<WorldGenerationLogicBase*> pLogicToAdd;
};

// Basic 1d noise, ignores the y value
EDITORCLASS()
class WorldGenerationLogicBasicNoise1D : public WorldGenerationLogicBase
{
	EDITORBODY()
public:

	//~ Begin WorldGenerationLogicBase Interface
	float DoLogic(RandSeed& inOutSeed, float x, float y) const override;
	//~ End WorldGenerationLogicBase Interface

protected:
	
	EDITORPROPERTY()
	BasicNoiseParams params;
};
