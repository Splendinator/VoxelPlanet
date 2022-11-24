#pragma once

#include "VectorPrimitiveBase.h"

#include <type_traits>

class UIObjectBase;

// Base class meant to be inherited by any UI class that can nest UI objects.
// This allows the UI be to hierarchical i.e a root UICanvas might contain a UIObjectHealthBar which itself is made up of a couple of UIObjectProgressBars
class UIObjectContainer
{
protected:

	void TickAllUIObjects(float deltaTime);
	void UninitAllUIObjects();

	template<typename TClass>
	TransientPtr<TClass> AddUIObject(VectorPrimitiveBase* pRoot, const std::string& label = "");

private:

	// Vector of all children objects inside this element. Children are deleted in UninitAllUIObjects()
	std::vector<UIObjectBase*> pChildren;
};

template<typename TClass>
TransientPtr<TClass> UIObjectContainer::AddUIObject(VectorPrimitiveBase* pRoot, const std::string& label /*= ""*/)
{
	static_assert(std::is_base_of<UIObjectBase, TClass>::value, "TClass must inherit from UIObjectBase");

	TransientPtr<VectorPrimitiveBase> pNewUIObjectRoot = label == "" ? TransientPtr<VectorPrimitiveBase>(pRoot) : pRoot->FindPrimitiveByLabel<VectorPrimitiveBase>(label);
	
	TClass* pNewUIObject = new TClass();
	
	pNewUIObject->BaseInit(pNewUIObjectRoot.Get());

	pChildren.push_back(pNewUIObject);

	return pNewUIObject;
}


