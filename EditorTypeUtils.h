#pragma once

#include "EditorTypePropertyFactoryClass.h"
#include "EditorTypePropertyFactoryFloat.h"
#include "EditorTypePropertyFactoryStruct.h"
#include "EditorTypePropertyFactoryVector.h"

namespace EditorTypeUtils
{
	static EditorTypePropertyFactoryFloat floatFactory;
	static EditorTypePropertyFactoryClass classFactory;
	static EditorTypePropertyFactoryStruct structFactory;
	static EditorTypePropertyFactoryVector vectorFactory;

	static EditorTypePropertyFactoryBase* propertyFactories[] = 
	{
		&floatFactory,
		&classFactory,
		&structFactory,
		&vectorFactory
	};
}
