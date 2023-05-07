#pragma once

/** EditorTypePropertyBase
*
* This class is used to represent a property of an editor type. See EditorTypeFactoryPropertiesBase.
*/
class EditorTypePropertyBase
{
public:
	EditorTypePropertyBase(const std::string& inName) : name(inName) {}
	
	virtual ~EditorTypePropertyBase() {};

	// Draw this property type in ImGUI
	virtual void DrawImGUI() = 0;

	// Deep copy this property type
	virtual EditorTypePropertyBase* DeepCopy() = 0;

protected:

	std::string name;

private:

};