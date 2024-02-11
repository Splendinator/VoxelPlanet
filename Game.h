#pragma once

#include <vector>

class ImGuiEditor;

// #TEMP: Remove these editor exposed stuff, just testing right now
EDITORSTRUCT()
struct FloatStruct
{
	EDITORBODY()
	
	EDITORPROPERTY()
	float a;

	EDITORPROPERTY()
	float b = 1.0f;
};

EDITORCLASS()
class SingleFloat
{
	EDITORBODY()

	EDITORPROPERTY()
	float value = 20.f;

	EDITORPROPERTY()
	SingleFloat* pNext = nullptr;

	EDITORPROPERTY()
	FloatStruct floatStruct;
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

