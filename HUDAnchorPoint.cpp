#include "pch.h"

#include "HUDAnchorPoint.h"

#include "DomMath/Vec2.h"

HUDAnchorPoint::HUDAnchorPoint(EHUDAnchorPoint AnchorPoint, float padding /*= 0.0f*/)
{
	const float minCoord = padding;
	const float maxCoord = 1.0f - padding;

	Vec2f anchorPoints[(int)EHUDAnchorPoint::COUNT];
	anchorPoints[(int)EHUDAnchorPoint::TopLeft] = Vec2f(minCoord, minCoord);
	anchorPoints[(int)EHUDAnchorPoint::TopCenter] = Vec2f(0.5f, minCoord);
	anchorPoints[(int)EHUDAnchorPoint::TopRight] = Vec2f(maxCoord, minCoord);
	anchorPoints[(int)EHUDAnchorPoint::CenterLeft] = Vec2f(minCoord, 0.5f);
	anchorPoints[(int)EHUDAnchorPoint::Center] = Vec2f(0.5f, 0.5f);
	anchorPoints[(int)EHUDAnchorPoint::CenterRight] = Vec2f(maxCoord, 0.5f);
	anchorPoints[(int)EHUDAnchorPoint::BottomLeft] = Vec2f(minCoord, maxCoord);
	anchorPoints[(int)EHUDAnchorPoint::BottomCenter] = Vec2f(0.5f, maxCoord);
	anchorPoints[(int)EHUDAnchorPoint::BottomRight] = Vec2f(maxCoord, maxCoord);
	
	anchorX = anchorPoints[(int)AnchorPoint].x;
	anchorY = anchorPoints[(int)AnchorPoint].y;
}

HUDAnchorPoint::HUDAnchorPoint(float x, float y)
{
	anchorX = x;
	anchorY = y;
}
