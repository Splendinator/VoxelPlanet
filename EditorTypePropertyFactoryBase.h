#pragma once

class EditorTypePropertyBase;

/** EditorTypePropertyFactoryBase
*
* This class is used to generate editor property types
*/
class EditorTypePropertyFactoryBase
{
public:

	// Should be overwritten to create an editor property type from an input stream
	virtual EditorTypePropertyBase* CreateType(std::ifstream& stream) = 0;
	
	// Should be overwritten to return the keyword for this type
	virtual std::string GetLabel() = 0;

protected:

private:

};
