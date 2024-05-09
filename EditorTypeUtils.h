#pragma once

#include "EditorTypePropertyFactoryBool.h"
#include "EditorTypePropertyFactoryClass.h"
#include "EditorTypePropertyFactoryEnum.h"
#include "EditorTypePropertyFactoryFloat.h"
#include "EditorTypePropertyFactoryInt.h"
#include "EditorTypePropertyFactoryString.h"
#include "EditorTypePropertyFactoryStruct.h"
#include "EditorTypePropertyFactoryVector.h"

namespace EditorTypeUtils
{
	static EditorTypePropertyFactoryFloat floatFactory;
	static EditorTypePropertyFactoryBool boolFactory;
	static EditorTypePropertyFactoryInt intFactory;
	static EditorTypePropertyFactoryString stringFactory;
	static EditorTypePropertyFactoryClass classFactory;
	static EditorTypePropertyFactoryStruct structFactory;
	static EditorTypePropertyFactoryVector vectorFactory;
	static EditorTypePropertyFactoryEnum enumFactory;

	static EditorTypePropertyFactoryBase* propertyFactories[] = 
	{
		&floatFactory,
		&boolFactory,
		&intFactory,
		&stringFactory,
		&classFactory,
		&structFactory,
		&vectorFactory,
		&enumFactory,
	};

}
