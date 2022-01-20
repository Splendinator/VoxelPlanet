#include "pch.h"

#include "VectorPrimitiveFactoryManager.h"

#include "DomImport/XMLUtils.h"
#include "VectorPrimitiveFactoryCircle.h"
#include "VectorPrimitiveFactoryLayer.h"
#include "VectorPrimitiveFactoryRectangle.h"

#include <fstream>
#include <string>

namespace 
{
	struct FactoryEntry
	{
		std::string label;
		VectorPrimitiveFactoryBase* pFactory = nullptr;
	};

	FactoryEntry factories[] =
	{
		{"g", new VectorPrimitiveFactoryLayer},
		{"ellipse", new VectorPrimitiveFactoryCircle},
	};
}

/// #TEMP: PRAGMA
#pragma optimize( "", off )
/*static*/ VectorPrimitiveLayer VectorPrimitiveFactoryManager::ReadInFile(const char* pFilePath)
{
	/// #TEMP: Move all of this parsing stuff to the layer and this function just creates the initial layer that recursively creates everything else

	std::fstream svgFile(pFilePath);

	DOMLOG_ERROR_IF(svgFile.bad(), "Cannot find file: ", pFilePath);

	VectorPrimitiveLayer layer;

	std::string tag;

	while (tag != "g")
	{
		tag = dmim::GetNextTag(svgFile);
	}

	layer.PopulateFromFile(svgFile);

	return layer;
}
#pragma optimize( "", on )
