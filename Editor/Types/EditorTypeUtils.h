#pragma once

#include "Editor/Types/Properties/EditorTypePropertyFactoryBool.h"
#include "Editor/Types/Properties/EditorTypePropertyFactoryClass.h"
#include "Editor/Types/Properties/EditorTypePropertyFactoryEnum.h"
#include "Editor/Types/Properties/EditorTypePropertyFactoryFloat.h"
#include "Editor/Types/Properties/EditorTypePropertyFactoryInstancedAssetPtr.h"
#include "Editor/Types/Properties/EditorTypePropertyFactoryInt.h"
#include "Editor/Types/Properties/EditorTypePropertyFactoryString.h"
#include "Editor/Types/Properties/EditorTypePropertyFactoryStruct.h"
#include "Editor/Types/Properties/EditorTypePropertyFactoryVector.h"
#include "Editor/Types/Properties/EditorTypePropertyFactoryDataCompositeProperty.h"

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
	static EditorTypePropertyFactoryInstancedAssetPtr instancedAssetFactory;
	static EditorTypePropertyFactoryDataCompositeProperty instancedDataCompositePropertyFactory;

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
		&instancedAssetFactory,
		&instancedDataCompositePropertyFactory,
	};

}
