#pragma once

#include <DomMath/Vec2.h>

#include "DomMath/Mat4.h"

/** RendererObject
*
* This class is used as an interface between the renderer and the game.
* It conatins functions like SetPosition() that the game can use to manipulate render objects
*/
class RendererObject
{
public:

	void SetPosition(float x, float y);
	void SetSize(float x, float y);
	void SetRenderPriority(float inRenderPriority); // Render priority should be between 0 and 1, 1.0f rendering on top of 0.0f
	void SetVisible(bool bInVisible) { bVisible = bInVisible; }

	Mat4f& GetModelMatrix() { return modelMatrix; }
	Vec2f GetPosition() const { return Vec2f(posX, posY); }
	Vec2f GetScale() const { return Vec2f(scaleX, scaleY); }
	bool IsVisible() const { return bVisible; }

protected:

	void UpdateModelMatrix();

private:

	float posX = 0.0f;
	float posY = 0.0f;
	float scaleX = 1.0f;
	float scaleY = 1.0f;
	float renderPriority = 1.0f; // Start off at 1 so it renders on top so it's more obvious if this hasn't been set up correctly
	
	bool bVisible = true; // Whether to render this

	Mat4f modelMatrix = Mat4f::Identity();
};
