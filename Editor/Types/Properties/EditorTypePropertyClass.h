#pragma once

#include "EditorTypePropertyBase.h"

class EditorTypeClass;

/** EditorTypePropertyClass
*
* This class is used to represent a class property (MyClass* pMyClass = nullptr)
* Classes are either instanced, meaning two pointers to the same data asset will result in two *copies* of the same class, or singletons which mean 2 pointers point to the same 1 object.
*
* IMPORTANT: Any system using raw pointer to an instanced property will be in charge of deleting afterwards 
*/
class EditorTypePropertyClass : public EditorTypePropertyBase
{
public:
	EditorTypePropertyClass();
	EditorTypePropertyClass(const std::string& propertyName, const std::string& inClassName, const std::string& inAssetName);

	//~ Begin EditorTypePropertyBase Interface
	void DrawImGUI() override;
	EditorTypePropertyBase* DeepCopy() override;
	bool CanReadFromFile(std::ifstream& file) const override;
	void ReadFromFile(std::ifstream& file) override;
	void WriteToFile(std::ofstream& file) override;
	void ForceSetValue(const ForceSetValueParams& params) override;
	//~ End EditorTypePropertyBase Interface

	void* GetValue() const;

	std::string GetClassName() const { return className; }

protected:

	void SetEditInlineNewClass(EditorTypeClass* editInlineNewClass);
	
	void OnEditInlineNewClassPropertyChanged(const OnPropertyChangedData& OnPropertyChangedData);
	
	// Name of the class of this property (SpellBase etc.)
	std::string className; 

	// Name of the actual asset (Fireball etc.).
	// This will be the name of the template subclass if we're using EClassMetadataFlags::EditInlineNew (SpellProjectile etc.) 
	std::string assetName;  

	// Deep copied class type. This contains all the properties etc.
	// This is only set if the class we're editing the class inline via EClassMetadataFlags::EditInlineNew.
	std::shared_ptr<EditorTypeClass> pEditInlineNewClass;
	

	// Delegate fired when EditInlineNew class property changes. Used to propogate changes up the chain
	DelegateClass<EditorTypePropertyClass, const OnPropertyChangedData&> onEditInlineNewClassPropertyChangedDelegate;
};

