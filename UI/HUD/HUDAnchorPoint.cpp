#include "pch.h"

#include "HUDAnchorPoint.h"

Vec2f HUDAnchorPoint::GetAnchorPoint(float padding) const
{
	DOMLOG_ERROR_IF(anchorPoint == EHUDAnchorPoint::COUNT, "Shouldn't be COUNT")
	
	const float minCoord = padding;
	const float maxCoord = 1.0f - padding;
	
	if (anchorPoint == EHUDAnchorPoint::NONE)
	{
		const float range = (maxCoord - minCoord);
		const float returnX = range * anchorX + minCoord;
		const float returnY = range * anchorY + minCoord;

		return {returnX, returnY};
	}
	else
	{
		static Vec2f anchorPoints[(int)EHUDAnchorPoint::COUNT];
		anchorPoints[(int)EHUDAnchorPoint::TopLeft] = Vec2f(minCoord, minCoord);
		anchorPoints[(int)EHUDAnchorPoint::TopCenter] = Vec2f(0.5f, minCoord);
		anchorPoints[(int)EHUDAnchorPoint::TopRight] = Vec2f(maxCoord, minCoord);
		anchorPoints[(int)EHUDAnchorPoint::CenterLeft] = Vec2f(minCoord, 0.5f);
		anchorPoints[(int)EHUDAnchorPoint::Center] = Vec2f(0.5f, 0.5f);
		anchorPoints[(int)EHUDAnchorPoint::CenterRight] = Vec2f(maxCoord, 0.5f);
		anchorPoints[(int)EHUDAnchorPoint::BottomLeft] = Vec2f(minCoord, maxCoord);
		anchorPoints[(int)EHUDAnchorPoint::BottomCenter] = Vec2f(0.5f, maxCoord);
		anchorPoints[(int)EHUDAnchorPoint::BottomRight] = Vec2f(maxCoord, maxCoord);
		
		return anchorPoints[(int)anchorPoint];
	}

	
}

float HUDAnchorPoint::GetAnchorPointX(float padding) const
{
	return GetAnchorPoint(padding).x;
}

float HUDAnchorPoint::GetAnchorPointY(float padding) const
{
	return GetAnchorPoint(padding).y;
}
