#pragma once

#include "Core/GameSystem.h"

// #NOTE: I created this class to test github copilot. I'm not sure if we need it vs using the InputSystem with EButton::LMB etc.

EDITORENUM()
enum class EMousePriority
{
	InGame,
	Menu,
	ImGuiEditor,
	COUNT
};

struct MouseButtonEventDelegateParams
{
	i32 mouseX;
	i32 mouseY;
};

struct MouseMoveEventDelegateParams
{
	// The mouse position
	i32 mouseX;
	i32 mouseY;

	// The delta of the mouse position
	i32 deltaX;
	i32 deltaY;
};

// This system acts as a game-level mouse input handler.
EDITORCLASS()
class MouseSystem : public GameSystem
{
	EDITORBODY()
public:

	//~ Begin GameSystem Interface
	void Init() override;
	void UnInit() override;
	void Tick(float deltaTime) override;
#ifdef DOMIMGUI
	void DrawImGui(float deltaTime) override;
#endif
	//~ End GameSystem Interface
	
	void PushPriority(EMousePriority priority) { bMousePriorityStack[(i32)priority] = true; }
	void PopPriority(EMousePriority priority) { bMousePriorityStack[(i32)priority] = false; }

private:

	EMousePriority GetHighestPriority() const;
	
	bool bMousePriorityStack[(i32)EMousePriority::COUNT] = {}; 
	
	// Delegates
	DelegateList<MouseButtonEventDelegateParams> onMouseDownDelegates[(i32)EMousePriority::COUNT];
	DelegateList<MouseButtonEventDelegateParams> onMouseUpDelegates[(i32)EMousePriority::COUNT];
	DelegateList<MouseMoveEventDelegateParams> onMouseMoveDelegates[(i32)EMousePriority::COUNT];
	
};
