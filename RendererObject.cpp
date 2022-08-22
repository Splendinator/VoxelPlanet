#include "pch.h"

#include "RendererObject.h"

void RendererObject::SetPosition(float x, float y)
{
	posX = x;
	posY = y;
	UpdateModelMatrix();
}

void RendererObject::SetSize(float x, float y)
{
	scaleX = x;
	scaleY = y;
	UpdateModelMatrix();
}

void RendererObject::SetRenderPriority(float inDepth)
{
	renderPriority = inDepth;
	UpdateModelMatrix();
}

void RendererObject::UpdateModelMatrix()
{
	modelMatrix = Mat4f::getScale({ scaleX, scaleY, 1.0f}) * Mat4f::getTranslation({ posX, posY, -renderPriority });
	modelMatrix.transpose();
}
