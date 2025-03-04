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
public:

	// Error checked that it resolves to a correct file, prefer this instead of manual concatenation
	static std::string ConcatenateFilePathChecked(std::string path, std::string fileName, std::string extension);
	static std::string ConcatenateSVGFilePathChecked(std::string path, std::string fileName) { return ConcatenateFilePathChecked(path, fileName, ".svg"); }
	
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

	// path to class .svg's
	EDITORPROPERTY()
	std::string rpgClassVisuals;

	// path to race .svg's
	EDITORPROPERTY()
	std::string rpgRaceVisuals;

	// path to skill .svg's
	EDITORPROPERTY()
	std::string rpgSkillVisuals;
};
