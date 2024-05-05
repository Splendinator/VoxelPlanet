#pragma once

class EditorTypePropertyBase;

struct OnPropertyChangedParams
{
	EditorTypePropertyBase* pProperty = nullptr;
	std::string oldValue;
	std::string newValue;
};

/** EditorTypePropertyBase
*
* This class is used to represent a property of an editor type. See EditorTypeFactoryPropertiesBase.
*/
class EditorTypePropertyBase
{
public:
	EditorTypePropertyBase() {}
	EditorTypePropertyBase(const std::string& inName) : name(inName) {}
	
	virtual ~EditorTypePropertyBase() {};

	// Draw this property type in ImGUI
	virtual void DrawImGUI() = 0;

	// This should return whether the input filestream is reading in this property from an asset. This should check the property name and type etc.
	// It's needed in-case the order of properties change in a class (new ones added, order changed, one removed, etc.)
	virtual bool CanReadFromFile(std::ifstream& file) const = 0;
	
	// Read and write from a file
	virtual void ReadFromFile(std::ifstream& file) = 0;
	virtual void WriteToFile(std::ofstream& file) = 0;

	// Deep copy this property type
	virtual EditorTypePropertyBase* DeepCopy() = 0;

	// This should be overwritten to set the value based off a string of that value.
	// This ***shouldn't*** call the onPropertyChanged delegate. It's used when setting the value in the editor
	virtual void ForceSetValue(const std::string& newValue) = 0;

	const std::string& GetName() { return name; }

	DelegateList<const OnPropertyChangedParams&> onPropertyChanged;

protected:

	std::string name;

private:

};