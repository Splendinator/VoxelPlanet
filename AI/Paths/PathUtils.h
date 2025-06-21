#pragma once

enum class EIntersectionExitDirectionFlags
{
	None = 0,
	
	Top = 1 << 0,
	Bottom = 1 << 1,
	Left = 1 << 2,
	Right = 1 << 3,

	TopLeft = Top | Left,
	TopRight = Top | Right,
	BottomLeft = Bottom | Left,
	BottomRight = Bottom | Right,
};
ENUM_CLASS_BITWISE_OPERATORS(EIntersectionExitDirectionFlags)

struct PathGridIntersectionPoints
{
	EIntersectionExitDirectionFlags exitDirection = EIntersectionExitDirectionFlags::None;
	
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
	PathGridIntersectionPoints GetGridIntersectionPointOfLine(Vec2i lineStartPosGrid, Vec2i lineEndPosGrid, Vec2i targetGridPos);
}
