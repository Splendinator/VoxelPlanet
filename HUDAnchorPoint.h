#pragma once
#include <DomMath/Vec2.h>

EDITORENUM()
enum class EHUDAnchorPoint
{
	NONE, 
	TopLeft,
	TopCenter,
	TopRight,
	CenterLeft,
	Center,
	CenterRight,
	BottomLeft,
	BottomCenter,
	BottomRight,
	COUNT
};

/** HUDAnchorPoint
 * 
 * Class to say where a HUD object will sit on the screen
 */
EDITORSTRUCT()
struct HUDAnchorPoint
{
	EDITORBODY()

	// Get the anchor point, (0,0) = top left.
	// Padding is percentage area around the screen HUD elements can't go.
	Vec2f GetAnchorPoint(float padding) const;
	float GetAnchorPointX(float padding) const;
	float GetAnchorPointY(float padding) const;
	
private:
	// If this is set, use these, else use the more fine-grained anchorX and anchorY
	EDITORPROPERTY()
	EHUDAnchorPoint anchorPoint = EHUDAnchorPoint::NONE;
	
	// Coordinates of the anchor point relative to screen size. i.e 0.5, 0.5 is the center of the screen.
	EDITORPROPERTY()
	float anchorX = 0.0f;

	EDITORPROPERTY()
	float anchorY = 0.0f;
};

