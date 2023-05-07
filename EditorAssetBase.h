#pragma once

/** EditorAssetBase
*
* This class is the base class for all editor assets. Assets are usually instances of types
*/
class EditorAssetBase
{
public:
	EditorAssetBase(const std::string& assetName) : name(assetName) {}
	virtual ~EditorAssetBase() {};

	// Draw the asset using ImGUI
	virtual void Draw() = 0;

	const std::string& GetName() { return name; }

protected:
	std::string name;
};
