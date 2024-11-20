#pragma once

#include "Core/GameSystem.h"

class InputContext;

/**
 *	Any system listening to user inputs should go through this system.
 *	It will fire delegates when inputs are fired and handle disabling certain inputs depending on context (no moving in a pause menu)
 */
EDITORCLASS()
class InputSystem : public GameSystem
{
	EDITORBODY()

public:
	//~ Begin GameSystem Interface
	void Init() override;
	void Tick(float deltaTime) override;
	void UnInit() override;
#ifdef DOMIMGUI
	void DrawImGui(float deltaTime) override;
#endif
	//~ End GameSystem Interface

	void PushInputContext(InputContext* pInputContext);
	void PopInputContext(InputContext* pInputContext);

private:
	
	// Input context that is always on
	EDITORPROPERTY()
	InputContext* pDefaultInputContext = nullptr;

	// Sorted vector of input contexts. The last entry in this array is the only context ticking
	std::vector<InputContext*> pActiveInputContexts;
	
	// #TODO: We need a DrawImGUI base function to test this perhaps?
};
