#pragma once

class AssetManager;

EDITORCLASS(Abstract, EditInlineNew)
class TestEditInlineNewClassBase
{
	EDITORBODY()

	virtual std::string GetName() { return "TestEditInlineNewClassBase"; }
};

EDITORCLASS()
class TestEditInlineNewClassFloat : public TestEditInlineNewClassBase
{
	EDITORBODY()
	
public:
	EDITORPROPERTY()
	float x = 20.f;

	virtual std::string GetName() override { return "TestEditInlineNewClassFloat"; }
};

EDITORCLASS()
class TestEditInlineNewClassInt : public TestEditInlineNewClassBase
{
	EDITORBODY()

	virtual std::string GetName() override { return "TestEditInlineNewClassInt"; }
	
public:
	EDITORPROPERTY()
	int x = 20;
};

EDITORCLASS()
class TestEditInlineNewClassXofY : public TestEditInlineNewClassInt
{
	EDITORBODY()

	virtual std::string GetName() override { return "TestEditInlineNewClassXofY"; }
	
public:
	EDITORPROPERTY()
	int y = 20;
};


EDITORCLASS()
class TestClass
{
	EDITORBODY()

public:
	
	EDITORPROPERTY()
	std::vector<TestEditInlineNewClassBase*> pTestEditInlineNewClasses;
};

namespace Game
{
	void Init();
	void UnInit();
	void Tick(float deltaTime);
	
	bool CanClose();
	
	AssetManager& GetAssetManager();
}

