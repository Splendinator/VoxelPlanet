#include "pch.h"

#include "EditorTypePropertyFactoryStruct.h"

#include "EditorTypePropertyStruct.h"
#include "EditorTypeStruct.h"
#include "Game.h"
#include "ImGuiEditor.h"

EditorTypePropertyBase* EditorTypePropertyFactoryStruct::CreateType(std::ifstream& stream)
{
	// Read in "struct MyStruct myStruct"
	std::string unused;
	std::string structName;
	std::string propertyName;
	stream >> unused >> structName >> propertyName;
	
	EditorTypeStruct* structTemplate = Game::Editor().FindStructTemplateType(structName);
	DOMLOG_ERROR_IF(structTemplate == nullptr, "Cannot find struct template object, are the assets being loaded in the wrong order?");
	
	return new EditorTypePropertyStruct(propertyName, structName, static_cast<EditorTypeStruct*>(structTemplate->DeepCopy()));
}

std::string EditorTypePropertyFactoryStruct::GetLabel()
{
	return "struct";	
}
