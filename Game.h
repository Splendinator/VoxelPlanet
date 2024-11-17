#pragma once

#include <vector>

class KeybindData;
class MenuSystem;
class DirectoryData;
class HUD;
class TextRenderSystem;
class ImGuiEditor;

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

