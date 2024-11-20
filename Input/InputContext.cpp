#include "pch.h"

#include "InputContext.h"

#include "InputAction.h"

void InputContext::Tick()
{
	for (InputActionBase* pInputAction : pInputActions)
	{
		pInputAction->Tick();
	}
}