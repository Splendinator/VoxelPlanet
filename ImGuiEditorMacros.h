#pragma once

// Goes at the top of headers that want to be omitted for some reason (like this one since the macros get picked up by the parser) 
#define EDITORIGNORE()

// Goes above classes that the editor should know about
#define EDITORCLASS()

// Goes above structs that the editor should know about
#define EDITORSTRUCT()

// Goes above enums that the editor should know about
#define EDITORENUM()

// Goes above properties of classes or structs that the editor should know about
#define EDITORPROPERTY()

// Has to go somewhere in the class/struct. Note the change of publicity scope and do not assume scope after this macro
#define EDITORBODY() public: \
static void* InitFromProperties(const std::vector<class EditorTypePropertyBase*>& properties); \
static void InitFromPropertiesSubset(void* pClass, const std::vector<EditorTypePropertyBase*>& properties, int& index);

namespace ImGuiEditorMacros
{
	static std::string editorClassString = "EDITORCLASS()";
	static std::string editorStructString = "EDITORSTRUCT()";
	static std::string editorEnumString = "EDITORENUM()";
	static std::string editorPropertyString = "EDITORPROPERTY()";
	static std::string editorIgnoreFileString = "EDITORIGNORE()";
};



