#pragma once

/** EditorTypeBase
*
* This class the base class for all editor types.
* Editor types are used to create and edit properties in the editor.
*/
class EditorTypeBase
{
public:
	
	virtual void DrawImGUI() = 0;

	// Name of this specific instance of the type. (i.e "healthPct" not "float")
	std::string name;
	
protected:

private:

};
