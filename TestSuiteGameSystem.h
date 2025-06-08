#pragma once

#include "Core/GameSystem.h"

// Class is just used for DrawImGui(). We use it to test functions that we add. 
EDITORCLASS()
class TestSuiteGameSystem : public GameSystem
{
	EDITORBODY()
	
public:
	
	//~ Begin GameSystem Interface
	void DrawImGui(float deltaTime) override;
	//~ End GameSystem Interface
	
};
