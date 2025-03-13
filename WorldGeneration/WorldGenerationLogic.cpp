#include "pch.h"

#include "WorldGenerationLogic.h"

// WorldGenerationLogicBase
float WorldGenerationLogicBase::BaseDoLogic(RandSeed& inOutSeed, float x, float y)
{
	float result = DoLogic(inOutSeed, x, y);

	if (bMutateSeedAfterLogic)
	{
		WorldGenerationUtils::MutateSeed(inOutSeed);	
	}

	return result;
}

// WorldGenerationLogicAdd
float WorldGenerationLogicAdd::DoLogic(RandSeed& inOutSeed, float x, float y) const
{
	float totalValue = 0.0f;
	
	for (WorldGenerationLogicBase* pLogic : pLogicToAdd)
	{
		if (pLogic)
		{
			totalValue += pLogic->BaseDoLogic(inOutSeed, x, y);
		}
	}
	
	return totalValue;
}

// WorldGenerationLogicBasicNoise1D
float WorldGenerationLogicBasicNoise1D::DoLogic(RandSeed& inOutSeed, float x, float y) const
{
	return WorldGenerationUtils::BasicNoise1D(x, inOutSeed, params);
}
