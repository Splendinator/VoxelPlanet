#pragma once
EDITORIGNORE()
class EditorTypePropertyBase;
namespace __Generated
{
	// This works for both structs and classes.
	extern std::unordered_map<std::string, void* (*)(const std::vector<EditorTypePropertyBase*>&)> stringToCreateObjectFunction;
}
