#pragma once
#include "Core/GameSystem.h"

class DirectoryData;
// System responsible for spell casting
EDITORCLASS()
class RPGSkillSystem : public GameSystem
{
	EDITORBODY()
public:

	//~ Begin GameSystem Interface
	void Init() override;
	void Tick(float deltaTime) override;
	void UnInit() override;
	//~ End GameSystem Interface


protected:

	EDITORPROPERTY()
	const DirectoryData* pDirectoryData = nullptr;
	
	// #TEMP: Testing alpha values
	EDITORPROPERTY()
	std::string redTransparentFileName;
	EDITORPROPERTY()
	std::string greenTransparentFileName;

	
};
