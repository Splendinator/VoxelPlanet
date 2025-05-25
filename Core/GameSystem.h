#pragma once

// Basic game system, the game will mostly constitute of many of these systems
EDITORCLASS(Abstract, Singleton)
class GameSystem
{
	EDITORBODY()
public:

	virtual void Init() {}
	virtual void UnInit() {}
	virtual void Tick(float deltaTime) {}

#ifdef DOMIMGUI
	// A window is already created, so this should just contain the content.
	virtual void DrawImGui(float deltaTime) {}
#endif
	
};
