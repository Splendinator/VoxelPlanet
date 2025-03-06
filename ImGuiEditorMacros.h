#pragma once

// Goes at the top of headers that want to be omitted for some reason (like this one since the macros get picked up by the parser) 
#define EDITORIGNORE()

// Goes above classes that the assetManager should know about
// Has these parameters:
// Abstract - This class cannot be created in the assetManager as an asset. Useful for classes that just want to be used as base classes like interfaces
// Instanced/Singleton - This describes how the class will be handled when instantiated in-game.
//	Instanced - Three pointers to the same asset will create 3 identical objects
//	Singleton - Three pointers to the same asset will create 1 shared object and all point to it.
//	If neither Instanced/Singleton are in the macro then it will use the base class / engine default
// EditInlineNew - When creating a new property pointer to this class you create it inline instead of linking to an external asset (like with structs)
#define EDITORCLASS(...)

// Goes above structs that the assetManager should know about
#define EDITORSTRUCT()

// Goes above enums that the assetManager should know about
#define EDITORENUM()

// Goes above properties of classes or structs that the assetManager should know about
#define EDITORPROPERTY()

// Has to go somewhere in the class/struct. Note the change of publicity scope.
#define EDITORBODY() public: \
static void* _InitFromProperties(const std::vector<class EditorTypePropertyBase*>& properties); \
static void _InitFromPropertiesSubset(void* pClass, const std::vector<EditorTypePropertyBase*>& properties, int& index); \
static void* _CreateEmptyObject();

namespace ImGuiEditorMacros
{
	static std::string editorClassString = "EDITORCLASS(";
	static std::string editorStructString = "EDITORSTRUCT(";
	static std::string editorEnumString = "EDITORENUM()";
	static std::string editorPropertyString = "EDITORPROPERTY()";
	static std::string editorIgnoreFileString = "EDITORIGNORE()";
};



