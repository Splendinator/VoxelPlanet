#pragma once

// Basic game system, the game will mostly constitute of many of these systems
EDITORCLASS(Abstract, Singleton)
class GameSystem
{
	EDITORBODY()
public:

	virtual void Init() {}
	virtual void Tick(float deltaTime) {}
	virtual void UnInit() {}
	
};
