#pragma once

#include "UI/UIObjects/UIObjectContainer.h"

#include "RenderPriorities.h"
#include "Renderer.h"
#include "VectorArt.h"

class RendererObject;

struct UICanvasInitParams
{
	// Size of quad in number of pixels
	float sizeX = 0.0f, sizeY = 0.0f;

	// render priority (UI/Menu) (see RenderPriorities.h)
	float renderPriority = RenderPriority::HUD;

	// UI or In-Game UI
	dmgf::ERenderObjectType type = dmgf::ERenderObjectType::InGame;

	// Vector art file path string (See DirectoryData)
	std::string filePath;
};

/** UICanvas
*
* Wrapper around a RenderObject and VectorArt to allow easy manipulation by the UI
* Optionally Tick() every frame if this UI Element needs it.
* Usage: Call CreateUIObject<UIObjectBase> to wrap UI objects around parts 
*/
class UICanvas : protected UIObjectContainer
{
public:
	UICanvas(UICanvasInitParams& initParams);
	~UICanvas();

	// UnInit to be called at end. Also called in destructer
	void UnInit();
	
	// Tick this canvas, this is optional
	void Tick(float deltaTime);
	
	// Set position of whole canvas
	void SetPosition(float x, float y);

	// Return the primitive directly under the cursor, or nullptr if not set
	const VectorPrimitiveBase* FindPrimitiveUnderCursor() const;
	
	// Find a labeled UIObject of class TClass on the canvas
	template<typename TClass>
	TransientPtr<TClass> GetUIObject(const std::string& label = "");

private:

	const UICanvas* GetCanvas() const final { return this; }
	
	bool IsInitialised() const { return pRenderObject != nullptr || pVectorArt != nullptr; }

	// Render object 
	RendererObject* pRenderObject = nullptr;

	// Vector art of entire element
	std::shared_ptr<VectorArt> pVectorArt = nullptr;
};

template<typename TClass>
TransientPtr<TClass> UICanvas::GetUIObject(const std::string& label /*= ""*/)
{
	return AddUIObject<TClass>(pVectorArt->GetRootLayer(), label);
}
