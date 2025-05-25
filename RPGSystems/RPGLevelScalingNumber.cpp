#include "pch.h"

#include "RPGLevelScalingNumber.h"

float RPGLevelScalingNumber::CalculateNumber(u32 level) const
{
	switch (scalingAlgorithm)
	{
	case ELevelScalingAlgorithm::Linear:
	{
		return baseValue + level * perLevelScaleFactor;
	}
	case ELevelScalingAlgorithm::Exponential:
	{
		float baseNum = baseValue;

		for (u32 i = 0; i < level; i++)
		{
			baseNum *= perLevelScaleFactor;
		}
		
		return baseNum;
	}
	}

	DOMLOG_ERROR("Invalid scaling algorithm", ENUMSTRING(ELevelScalingAlgorithm, scalingAlgorithm));
	return 0.0f;
}
