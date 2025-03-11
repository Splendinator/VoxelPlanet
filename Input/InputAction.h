#pragma once

#include "InputKey.h"

class InputActionBase;

struct OnInputActionDelegateParams
{
	InputActionBase* pInputAction = nullptr;
};

// A single action fired when the corresponding input conditions are met.
// There should be one of these for every action "MoveNorth" "OpenMenu" etc.
EDITORCLASS(Abstract, Singleton)
class InputActionBase
{
	EDITORBODY()

public:

	// Children should override this to change bActivated
	virtual void Tick() {}
	
	bool IsActive() const { return bActivated; }
	
	// Fires when the action should activate
	DelegateList<const OnInputActionDelegateParams&> onActionActivatedDelegates;

	// Fires when the action deactivates, this will only work for "stateful" input actions like "Hold"
	DelegateList<const OnInputActionDelegateParams&> onActionDeactivatedDelegates;

protected:
	
	// Base key that must be pressed
	EDITORPROPERTY()
	EInputKey baseKey = EInputKey::NONE;

	// Whether CTRL must be being held as well
	EDITORPROPERTY()
	bool bCtrl = false;

	// Whether SHIFT must be being held as well
	EDITORPROPERTY()
	bool bShift = false;

	// Whether ALT must be being held as well 
	EDITORPROPERTY()
	bool bAlt = false;

	// Children should make this true while the action is active, even non-stateful actions like "Press" should make it true for a frame
	bool bActivated = false;
};

// Input action that fires once when hitting the button
EDITORCLASS()
class InputActionPress : public InputActionBase
{
	EDITORBODY()

protected:
	//~ Begin InputActionBase Interface
	void Tick() override;
	//~ End InputActionBase Interface
};
