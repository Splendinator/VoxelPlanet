#pragma once

#include "Graphics/VectorArt/VectorPrimitiveLayer.h"

#include <type_traits>

class UICanvas;
class UIObjectBase;

// Base class meant to be inherited by any UI class that can nest UI objects.
// This allows the UI be to hierarchical i.e a root UICanvas might contain a UIObjectHealthBar which itself is made up of a couple of UIObjectProgressBars
class UIObjectContainer
{
	EDITORBODY()
protected:

	void TickAllUIObjects(float deltaTime);
	void UninitAllUIObjects();

	template<typename TClass>
	TransientPtr<TClass> AddUIObject(VectorPrimitiveLayer* pRoot, const std::string& label = "");

	virtual const UICanvas* GetCanvas() const = 0;
	
private:

	// Vector of all children objects inside this element. Children are deleted in UninitAllUIObjects()
	std::vector<UIObjectBase*> pChildren;
};

template<typename TClass>
TransientPtr<TClass> UIObjectContainer::AddUIObject(VectorPrimitiveLayer* pRoot, const std::string& label /*= ""*/)
{
	static_assert(std::is_base_of<UIObjectBase, TClass>::value, "TClass must inherit from UIObjectBase");

	VectorPrimitiveLayer* pNewUIObjectRoot = label == "" ? pRoot : pRoot->FindLayerByLabel(label);
	
	TClass* pNewUIObject = new TClass();
	
	pNewUIObject->BaseInit(pNewUIObjectRoot, GetCanvas());

	pChildren.push_back(pNewUIObject);

	return pNewUIObject;
}


