#include "pch.h"

#include "UICanvas.h"

#include "RendererObject.h"

UICanvas::UICanvas(UICanvasInitParams& initParams)
{
	pVectorArt = std::make_shared<VectorArt>(initParams.filePath);

	pRenderObject = dmgf::AddObjectFromVectorArt(pVectorArt, initParams.type);
	pRenderObject->SetRenderPriority(initParams.renderPriority);
	pRenderObject->SetSize(initParams.sizeX, initParams.sizeY);
}

UICanvas::~UICanvas()
{
	if (IsInitialised())
	{
		Uninit();
	}
}

void UICanvas::Tick(float deltaTime)
{
	TickAllUIObjects(deltaTime);
}

void UICanvas::Uninit()
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
