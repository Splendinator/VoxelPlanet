#pragma once

/** DirectoryData
 *
 * Just a nice centralised place to hold directories to things like textures etc.
 * All file paths are relative to project's base
 */
EDITORCLASS()
class DirectoryData
{
	EDITORBODY()

	// path to the font files
	EDITORPROPERTY()
	std::string fonts;

	// path to the hudObject files
	EDITORPROPERTY()
	std::string hudObjects;
};
