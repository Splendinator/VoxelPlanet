#pragma once

#include <vector>

class ImGuiEditor;

EDITORCLASS()
class SingleFloat
{
	EDITORBODY()

	EDITORPROPERTY()
	float value = 20.f;

	EDITORPROPERTY()
	SingleFloat* pNext = nullptr;
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

