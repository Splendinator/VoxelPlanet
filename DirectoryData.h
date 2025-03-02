#pragma once

/** DirectoryData
 *
 * Just a nice centralised place to hold directories to things like textures etc.
 * All file paths are relative to project's base
 */
EDITORCLASS(Singleton)
class DirectoryData
{
	EDITORBODY()

	// path to the font files
	EDITORPROPERTY()
	std::string fonts;

	// path to the hudObject .svgs
	EDITORPROPERTY()
	std::string hudObjects;

	// path to the menu .svg's 
	EDITORPROPERTY()
	std::string menus;

	// path to the shared UI .svg's (icons and stuff) 
	EDITORPROPERTY()
	std::string sharedUI;
};
