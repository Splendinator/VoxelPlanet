#include "pch.h"

#include "InputAction.h"

#include "DomWindow/DomWindow.h"

// InputActionPress
void InputActionPress::Tick()
{
	if (bActivated)
	{
		bActivated = false;
	}
	else
	{
		if (bCtrl && !dmwi::isHeld(dmwi::CTRL)) { return; }
		if (bAlt && !dmwi::isHeld(dmwi::LALT)) { return; }
		if (bShift && !dmwi::isHeld(dmwi::SHIFT)) { return; }

		if (dmwi::isPressed((dmwi::Button)baseKey))
		{
			bActivated = true;
			OnActionActivated.Invoke(OnInputActionDelegateParams());
		}
	}
}
