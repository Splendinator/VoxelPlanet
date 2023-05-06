#include "pch.h"

#ifdef DOMIMGUI

#include "ImGuiEditor.h"

#include <filesystem>

#include "DomImport/DomImport.h"
#include "EditorTypeBase.h"
#include "EditorTypeFactoryBase.h"
#include "EditorTypeFactoryClass.h"
#include "ImGuiEditorGlobals.h"

/// #TEMP: PRAGMA
#pragma optimize( "", off )
void ImGuiEditor::Init()
{	
	CreateTemplateTypes(ImGuiEditorGlobals::codeFilesBaseDirectory + "\\" + ImGuiEditorGlobals::editorTypesOutputFile);
}

void ImGuiEditor::Uninit()
{
	
}

void ImGuiEditor::Tick()
{
	ImGui::SetWindowPos({ 0,0 }, ImGuiCond_FirstUseEver);
	ImGui::SetWindowSize({ 400,400 }, ImGuiCond_FirstUseEver);
	ImGui::Begin("Editor");
	
	for (const auto& typePair : templateTypes)
	{
		typePair.second->DrawImGUI();
	}
	
	ImGui::End();
}

void ImGuiEditor::CreateTemplateTypes(const std::string& typesFile)
{
	std::ifstream inputFile(typesFile);

	EditorTypeFactoryClass editorTypeFactoryClass;
	
	EditorTypeFactoryBase* pEditorTypeFactories[] = 
	{
		&editorTypeFactoryClass
	};

	while (!inputFile.eof())
	{
		std::string nextString;
		inputFile >> nextString;

		if (nextString == "")
		{
			// Edge case - last line of file
			continue;
		}

		bool bFoundType = false;
		for (EditorTypeFactoryBase* pEditorFactory : pEditorTypeFactories)
		{
			if (pEditorFactory->GetKeyword() == nextString)
			{
				EditorTypeBase* pEditorType = pEditorFactory->CreateType(inputFile);

				DOMLOG_ERROR_IF(pEditorType == nullptr, "Failed to create editor type for type", pEditorFactory->GetKeyword());
				DOMLOG_ERROR_IF(pEditorType->name == "", "Editor type created with no name", pEditorFactory->GetKeyword());

				bFoundType = true;
				templateTypes.insert({ pEditorType->name, pEditorType });

				break;
			}
		}

		DOMLOG_ERROR_IF(!bFoundType, "Failed to find editor type factory for keyword", nextString);
	}
}

#endif //~ DOMIMGUI
#pragma optimize( "", on )
