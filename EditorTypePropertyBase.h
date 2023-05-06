#pragma once

/** EditorTypePropertyBase
*
* This class is used to represent a property of an editor type. See EditorTypeFactoryPropertiesBase.
*/
class EditorTypePropertyBase
{
public:
	EditorTypePropertyBase(const std::string& inName) : name(inName) {}

	virtual void DrawImGUI() = 0;

protected:

	std::string name;

private:

};