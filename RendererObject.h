#pragma once

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

	Mat4f& GetModelMatrix() { return modelMatrix; }

protected:

	void UpdateModelMatrix();

private:

	float posX = 0.0f;
	float posY = 0.0f;
	float scaleX = 0.0f;
	float scaleY = 0.0f;
	float renderPriority = 0.0f;

	Mat4f modelMatrix = Mat4f::Identity();
};
