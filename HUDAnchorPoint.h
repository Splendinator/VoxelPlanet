#pragma once

enum class EHUDAnchorPoint
{
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
class HUDAnchorPoint
{
public:
	HUDAnchorPoint() : HUDAnchorPoint(EHUDAnchorPoint::TopLeft, 0.0f) {};
	HUDAnchorPoint(EHUDAnchorPoint AnchorPoint, float padding = 0.0f); // Padding is a frac of the screen size
	HUDAnchorPoint(float x, float y);

	// Coordinates of the anchor point relative to screen size. i.e 0.5, 0.5 is the center of the screen.
	float anchorX = 0.0f;
	float anchorY = 0.0f;
};

