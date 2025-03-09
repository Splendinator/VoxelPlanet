#pragma once
EDITORIGNORE()
class EditorTypePropertyBase;
namespace __Generated
{
	// This works for both structs and classes.
	extern std::unordered_map<std::string, void* (*)(const std::vector<EditorTypePropertyBase*>&)> stringToCreateObjectFunction;
	
	// These two combined do the same thing as the top one. These are useful if you want to defer initialising the properties for some reason (we use it for singletons to prevent circular dependencies)
	extern std::unordered_map<std::string, void* (*)()> stringToCreateEmptyObjectFunction;
	extern std::unordered_map<std::string, void (*)(void*, const std::vector<EditorTypePropertyBase*>&, int&)> stringToInitialiseExistingObjectFunction;

	// Used to delete the void* returned by the above create functions.
	extern std::unordered_map<std::string, void (*)(void*)> stringToDeleteObjectFunction;
}
