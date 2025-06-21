#include "pch.h"

#include "PathUtils.h"

Vec2f PathUtils::GetCenterOfGrid(Vec2i gridPos)
{
	return {(float)gridPos.x + 0.5f, (float)gridPos.y + 0.5f};
}

PathGridIntersectionPoints PathUtils::GetGridIntersectionPointOfLine(Vec2i lineStartPosGrid, Vec2i lineEndPosGrid, Vec2i targetGridPos)
{
	// I don't know if I'm overcomplicating the shit out of this, it feels like it shouldn't be ~80 lines of code just to see where a line intersects an AABB
	
	PathGridIntersectionPoints intersectionPoint;

	if (lineStartPosGrid == lineEndPosGrid)
	{
		// Line is a point, so no intersection with grid
		return intersectionPoint;
	}

	const Vec2f targetPosTopLeft = {(float)targetGridPos.x, (float)targetGridPos.y};
	const Vec2f targetPosBottomRight = {(float)targetGridPos.x + 1.0f, (float)targetGridPos.y + 1.0f};
	
	const Vec2f lineStartPos = GetCenterOfGrid(lineStartPosGrid);
	const Vec2f lineEndPos = GetCenterOfGrid(lineEndPosGrid);

	const bool bLineMovingRight = lineStartPos.x < lineEndPos.x;
	const bool bLineMovingDown = lineStartPos.y < lineEndPos.y;
	
	const float lineMinX = bLineMovingRight ? lineStartPos.x : lineEndPos.x;
	const float lineMaxX = bLineMovingRight ? lineEndPos.x : lineStartPos.x;
	const float lineMinY = bLineMovingDown ? lineStartPos.y : lineEndPos.y;
	const float lineMaxY = bLineMovingDown ? lineEndPos.y : lineStartPos.y;
	
	// 0.0001f to prevent division by zero
	const float lineXLength = std::max(lineMaxX - lineMinX, 0.00001f); 
	const float lineYLength = std::max(lineMaxY - lineMinY, 0.00001f); 

	const float targetLeftLineIntersectFraction = (targetPosTopLeft.x - lineMinX) / lineXLength;
	const float targetRightLineIntersectFraction = (targetPosBottomRight.x - lineMinX) / lineXLength;
	const float targetTopLineIntersectFraction = (targetPosTopLeft.y - lineMinY) / lineYLength;
	const float targetBottomLineIntersectFraction = (targetPosBottomRight.y - lineMinY) / lineYLength;

	const bool bDirectionsAligned = (bLineMovingRight && bLineMovingDown) || (!bLineMovingRight && !bLineMovingDown);
	
	// Check left side intersection
	if (targetLeftLineIntersectFraction > 0.0f && targetLeftLineIntersectFraction < 1.0f)
	{
		const float yIntersect = lineMinY + lineYLength * (bDirectionsAligned ? targetLeftLineIntersectFraction : (1.0f - targetLeftLineIntersectFraction));
		if (yIntersect >= targetPosTopLeft.y && yIntersect <= targetPosBottomRight.y)
		{
			Vec2f& target = bLineMovingRight ? intersectionPoint.entryPoint : intersectionPoint.exitPoint;
			target.x = lineMinX + lineXLength * targetLeftLineIntersectFraction;
			target.y = yIntersect;

			if (!bLineMovingRight)
			{
				intersectionPoint.exitDirection = (intersectionPoint.exitDirection | EIntersectionExitDirectionFlags::Left);
			}
		}
	}

	// Check right side intersection
	if (targetRightLineIntersectFraction > 0.0f && targetRightLineIntersectFraction < 1.0f)
	{
		const float yIntersect = lineMinY + lineYLength * (bDirectionsAligned ? targetRightLineIntersectFraction : (1.0f - targetRightLineIntersectFraction));
		if (yIntersect >= targetPosTopLeft.y && yIntersect <= targetPosBottomRight.y)
		{
			Vec2f& target = bLineMovingRight ? intersectionPoint.exitPoint : intersectionPoint.entryPoint;
			target.x = lineMinX + lineXLength * targetRightLineIntersectFraction;
			target.y = yIntersect;

			if (bLineMovingRight)
			{
				intersectionPoint.exitDirection = (intersectionPoint.exitDirection | EIntersectionExitDirectionFlags::Right);
			}
		}
	}

	// Check top side intersection
	if (targetTopLineIntersectFraction > 0.0f && targetTopLineIntersectFraction < 1.0f)
	{
		const float xIntersect = lineMinX + lineXLength * (bDirectionsAligned ? targetTopLineIntersectFraction : (1.0f - targetTopLineIntersectFraction));
		if (xIntersect >= targetPosTopLeft.x && xIntersect <= targetPosBottomRight.x)
		{
			Vec2f& target = bLineMovingDown ? intersectionPoint.entryPoint : intersectionPoint.exitPoint;
			target.x = xIntersect;
			target.y = lineMinY + lineYLength * targetTopLineIntersectFraction;

			if (!bLineMovingDown)
			{
				intersectionPoint.exitDirection = (intersectionPoint.exitDirection | EIntersectionExitDirectionFlags::Top);
			}
		}
	}

	// Check bottom side intersection
	if (targetBottomLineIntersectFraction > 0.0f && targetBottomLineIntersectFraction < 1.0f)
	{
		const float xIntersect = lineMinX + lineXLength * (bDirectionsAligned ? targetBottomLineIntersectFraction : (1.0f - targetBottomLineIntersectFraction));
		if (xIntersect >= targetPosTopLeft.x && xIntersect <= targetPosBottomRight.x)
		{
			Vec2f& target = bLineMovingDown ? intersectionPoint.exitPoint : intersectionPoint.entryPoint;
			target.x = xIntersect;
			target.y = lineMinY + lineYLength * targetBottomLineIntersectFraction;

			if (bLineMovingDown)
			{
				intersectionPoint.exitDirection = (intersectionPoint.exitDirection | EIntersectionExitDirectionFlags::Bottom);
			}
		}
	}
	
	return intersectionPoint;
}
