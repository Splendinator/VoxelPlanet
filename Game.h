#pragma once

#include <vector>

class ImGuiEditor;


// #TEMP: Remove these editor exposed stuff, just testing right now
EDITORCLASS()
class SingleFloat
{
	EDITORBODY()
	
	EDITORPROPERTY()
	std::vector<float> values;
};

namespace Game
{
	void Init();
	void UnInit();

	void Tick(float deltaTime);

	bool CanClose();

#ifdef DOMIMGUI
	ImGuiEditor& Editor();
#endif
}

