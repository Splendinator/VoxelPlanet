#include "pch.h"

#ifdef DOMIMGUI

#include "ImGuiEditor.h"

#include "DomImport/DomImport.h"

#include <filesystem>

#include "CodeParseTokenBase.h"
#include "CodeParseTokenFactoryBase.h"
#include "CodeParseTokenFactoryClass.h"
#include "CodeParseTokenFactoryProperty.h"

static std::string codeFilesBaseDirectory = "C:\\Users\\Dominic\\Desktop\\ImGuiEditorTest"; /// #TEMP: Change this to the project directory once tested
static std::string editorTypesOutputFile = "EditorTypes.txt";

/// #TEMP: PRAGMA
#pragma optimize( "", off )
void ImGuiEditor::Init()
{
	std::vector<std::string> codeFiles = GetCodeFilesInDirectory(codeFilesBaseDirectory);
	
	// Gather all tokens
	std::vector<CodeParseTokenBase*> allTokens;
	for (const std::string& codeFile : codeFiles)
	{
		Parse(codeFile, allTokens);
	}

	// Write to file
	std::ofstream outputFile(codeFilesBaseDirectory + "\\" + editorTypesOutputFile);
	for (CodeParseTokenBase* token : allTokens)
	{
		token->WriteToFile(outputFile);
		outputFile << std::endl;
	}
}

void ImGuiEditor::Uninit()
{

}

void ImGuiEditor::Tick()
{

}

std::vector<std::string> ImGuiEditor::GetCodeFilesInDirectory(const std::string& directory)
{
	std::vector<std::string> codeFiles;

	for (const auto& entry : std::filesystem::directory_iterator(directory))
	{
		if (entry.is_regular_file())
		{
			const std::string extension = entry.path().extension().string();
			if (extension == ".cpp" || extension == ".h")
			{
				codeFiles.push_back(entry.path().string());
			}
		}
		else if (entry.is_directory())
		{
			std::vector<std::string> subDirectoryCodeFiles = GetCodeFilesInDirectory(entry.path().string());
			codeFiles.insert(codeFiles.end(), subDirectoryCodeFiles.begin(), subDirectoryCodeFiles.end());
		}
	}

	return codeFiles;
}

void ImGuiEditor::Parse(std::string codeFile, std::vector<CodeParseTokenBase*>& allTokens)
{
	dmut::HeapAllocSize<char> code = dmim::importText(codeFile.c_str());
	std::stringstream stream(code.RawPtr());
	
	// Set up token factories
	CodeParseTokenFactoryClass codeParseTokenFactoryClass;
	CodeParseTokenFactoryProperty codeParseTokenFactoryPropertyClass;
	
	codeParseTokenFactoryPropertyClass.SetRequiredPrecedingToken(&codeParseTokenFactoryClass);
	
	CodeParseTokenFactoryBase* pFactories[] =
	{
		&codeParseTokenFactoryClass,
		&codeParseTokenFactoryPropertyClass
	};

	// Parse code
	std::stack<CodeParseTokenFactoryBase*> activeScopedFactories; // Token factories that are currently active (i.e. we are inside a class, struct, etc)
	
	while (!stream.eof())
	{
		std::string nextString;
		stream >> nextString;

		// Check for end of scope
		if (!activeScopedFactories.empty())
		{
			CodeParseTokenFactoryBase* pScopedTokenType = activeScopedFactories.top();
			std::string endString = pScopedTokenType->GetEndString();

			if (nextString.size() >= endString.size() && nextString.substr(0, endString.size()) == endString)
			{
				activeScopedFactories.pop();
				continue;
			}
		}

		// Check factories
		for (CodeParseTokenFactoryBase* pFactory : pFactories)
		{
			if (pFactory->CanFactoryBeUsed(activeScopedFactories) && pFactory->IsKeyword(nextString))
			{
				CodeParseTokenBase* pToken = pFactory->CreateToken(nextString, stream);
				if (pToken)
				{
					allTokens.push_back(pToken);
					
					if (pFactory->IsScopedTokenType())
					{
						activeScopedFactories.push(pFactory);
					}

					break;
				}
				else
				{
					DOMLOG_ERROR("Cannot create token for keyword:", nextString, "in file", codeFile);
				}
			}
		}
	}

	DOMLOG_ERROR_IF(!activeScopedFactories.empty(), "Scoped factories not detecting end of scope?")
}

#endif //~ DOMIMGUI
#pragma optimize( "", on )
