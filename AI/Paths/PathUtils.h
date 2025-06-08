#pragma once

struct PathGridIntersectionPoint
{
	// -1.0f means invalid
	Vec2f entryPoint = {-1.0f, -1.0f}; 
	Vec2f exitPoint = {-1.0f, -1.0f};

	bool DoesEnter() const { return entryPoint.x >= 0.0f && entryPoint.y >= 0.0f; }
	bool DoesExit() const { return exitPoint.x >= 0.0f && exitPoint.y >= 0.0f; }
};

namespace PathUtils
{
	// Returns the center of a grid square (by default gridPos is the top-left corner of the square)  
	Vec2f GetCenterOfGrid(Vec2i gridPos);
	
	// Returns the points that a path enters and leaves a grid square, or 0,0,0 if it doesn't
	PathGridIntersectionPoint GetGridIntersectionPointOfLine(Vec2i lineStartPosGrid, Vec2i lineEndPosGrid, Vec2i targetGridPos);
}
