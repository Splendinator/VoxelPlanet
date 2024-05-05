#pragma once

#include <vector>

class ImGuiEditor;

// #TEMP: Remove these editor exposed stuff, just testing right now
EDITORCLASS()
class SingleFloat
{
	EDITORBODY()
	
	EDITORPROPERTY()
	float floatOne;
	
	EDITORPROPERTY()
	float floatTwo;
	
	EDITORPROPERTY()
	std::vector<std::string> strings;

	EDITORPROPERTY()
	std::vector<int> integers;

	EDITORPROPERTY()
	std::vector<bool> bools;
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

