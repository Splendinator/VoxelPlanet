#pragma once

class InputActionBase;

// InputContexts define which InputActions are available to fire at a given time.
// e.g there might "InputContext_Menu" that stops "InputAction_MoveNorth" but contains "InputAction_CloseMenu"
EDITORCLASS(Singleton)
class InputContext
{
	EDITORBODY()
public:
	
	void Tick();
	
	int GetPriority() const {return priority;}

	const std::vector<InputActionBase*>& GetInputActions() {return pInputActions;}
	
protected:
	// Priority of this input context, highest priority active context will be the only one working.
	EDITORPROPERTY()
	int priority = 0;
	
	// List of input actions that activate while this context is active
	EDITORPROPERTY()
	std::vector<InputActionBase*> pInputActions;
};
