#pragma once

struct OnPropertyChangedParams;

/** EditorTypeBase
*
* This class the base class for all editor types.
* Editor types are used to create and edit properties in the editor.
*/
class EditorTypeBase
{
public:
	EditorTypeBase() {};
	virtual ~EditorTypeBase() {};

	// Draw the property to ImGUI so we can edit it in the editor.
	virtual void DrawImGUI() = 0;

	// Deep copy this type
	virtual EditorTypeBase* DeepCopy() = 0;

	// Read and write from a file
	virtual void ReadFromFile(std::ifstream& file) = 0;
	virtual void WriteToFile(std::ofstream& file) = 0;
	
	// #TODO: Make this protected
	// Name of this specific instance of the type. (i.e "healthPct" not "float")
	std::string name;

protected:

private:

};
