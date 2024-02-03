#pragma once

// Goes at the top of headers that want to be omitted for some reason (like this one since the macros get picked up by the parser) 
#define EDITORIGNORE()

// Goes above classes that the editor should know about
#define EDITORCLASS()

// Goes above properties of classes or structs that the editor should know about
#define EDITORPROPERTY()

// Has to go somewhere in the class/struct. Note the change of publicity scope and do not assume scope after this macro
#define EDITORBODY() public: \
static void* InitFromAsset(class EditorAssetBase* pAsset);

namespace ImGuiEditorMacros
{
	static std::string editorClassString = "EDITORCLASS()";
	static std::string editorPropertyString = "EDITORPROPERTY()";
	static std::string editorIgnoreFileString = "EDITORIGNORE()";
};



