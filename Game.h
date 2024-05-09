#pragma once

#include <vector>

class TextRenderSystem;
class DirectoryData;

// #TEMP: Remove this
EDITORENUM()
enum class EMyEnum
{
	Red,
	Green,
};

EDITORSTRUCT()
struct MyStruct
{
	EDITORBODY()

	EDITORPROPERTY()
	int x;
};

// This class will be the root data asset that references all other data assets directly or indirectly.
// A single instance of this class is explicitly created when the game starts and any other classes it
// referenced through EDITORPROPERTIES will also be loaded 
EDITORCLASS()
class GameAssets
{
	EDITORBODY()

public:

	// System to render text on screen
	EDITORPROPERTY()
	TextRenderSystem* pTextRenderSystem = nullptr;

	// Data of the relevant directories we're using
	EDITORPROPERTY()
	DirectoryData* pDirectoryData = nullptr;

	// #TEMP: Remove
	EDITORPROPERTY()
	EMyEnum myEnum = EMyEnum::Green;
};

class ImGuiEditor;

namespace Game
{
	void Init();
	void UnInit();

	void Tick(float deltaTime);

	bool CanClose();

	GameAssets& GetGameAssets();

#ifdef DOMIMGUI
	ImGuiEditor& Editor();
#endif
}

