#include "pch.h"

#include "UICanvas.h"

#include "RendererObject.h"

#include "DomWindow/DomWindow.h"

UICanvas::UICanvas(UICanvasInitParams& initParams)
{
	pVectorArt = std::make_shared<VectorArt>(initParams.filePath.c_str());

	pRenderObject = dmgf::AddObjectFromVectorArt(pVectorArt, initParams.type);
	pRenderObject->SetRenderPriority(initParams.renderPriority);
	pRenderObject->SetSize(initParams.sizeX, initParams.sizeY);
	pRenderObject->SetPosition(initParams.posX, initParams.posY);
	pRenderObject->SetVisible(initParams.bStartVisible);
}

UICanvas::~UICanvas()
{
	if (IsInitialised())
	{
		UnInit();
	}
}

void UICanvas::Tick(float deltaTime)
{
	TickAllUIObjects(deltaTime);
}

void UICanvas::UnInit()
{
	UninitAllUIObjects();
	
	dmgf::RemoveObject(pRenderObject);

	pRenderObject = nullptr;
	
	pVectorArt.reset();
}

void UICanvas::SetPosition(float x, float y)
{
	pRenderObject->SetPosition(x, y);
}

void UICanvas::SetVisible(bool bVisible)
{
	pRenderObject->SetVisible(bVisible);
}

Vec2f UICanvas::GetScreenSpacePosition() const
{
	return pRenderObject->GetPosition();
}

Vec2f UICanvas::GetScreenSpaceScale() const
{
	const Vec2f renderObjectSize = pRenderObject->GetScale();
	const Vec2f vectorArtSize = {(float)pVectorArt->GetPageWidth() ,(float)pVectorArt->GetPageHeight()}; 

	return renderObjectSize / vectorArtSize;
}

const VectorPrimitiveBase* UICanvas::FindPrimitiveUnderCursor() const
{
	// #TODO: Likely want to cache the result of this for the rest of the frame
	
	Vec2i mousePos = dmwi::GetMousePos();

	// mousePos is relative to the top left of the window.
	// We need to translate it such that Vec2i(0,0) would be top left of the vector art, and Vec2i(VectorArt::PageWidth, VectorArt::PageHeight) would be bottom right.
	// i.e we need to change from render object space to vector art space

	// 1. Make mousePos relative to the top left of the render object
	mousePos.x -= (int)pRenderObject->GetPosition().x;
	mousePos.y -= (int)pRenderObject->GetPosition().y;

	// 2. Re-scale to change from render object co-ordinate space to vector art co-ordinate space  
	const float vectorArtPageWidth = (float)pVectorArt->GetPageWidth();
	const float vectorArtPageHeight = (float)pVectorArt->GetPageHeight();
	const Vec2f renderObjectScale = pRenderObject->GetScale();

	const float mouseXPosScaleMultiplier = vectorArtPageWidth / renderObjectScale.x;
	const float mouseYPosScaleMultiplier = vectorArtPageHeight / renderObjectScale.y;

	mousePos.x = (int)((float)mousePos.x * mouseXPosScaleMultiplier);
	mousePos.y = (int)((float)mousePos.y * mouseYPosScaleMultiplier);
	
	return pVectorArt->GetRootLayer()->FindPrimitiveUnderCursor(mousePos); 
}
