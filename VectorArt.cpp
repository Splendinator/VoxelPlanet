#include "pch.h"

#include "VectorArt.h"

#include "DomImport/XMLUtils.h"
#include "VectorPrimitiveFactoryCircle.h"
#include "VectorPrimitiveFactoryLayer.h"
#include "VectorPrimitiveFactoryRectangle.h"

#include <fstream>
#include <string>

VectorArt::VectorArt(const char* pFilePath)
{
	std::fstream svgFile(pFilePath);

	DOMLOG_ERROR_IF(svgFile.bad(), "Cannot find file: ", pFilePath);

	pRootLayer = std::make_unique<VectorPrimitiveLayer>();

	std::string tag;

	// Meta data
	while (tag != "svg")
	{
		tag = dmim::GetNextTag(svgFile);
	}
	tag = dmim::GetNextAttribute(svgFile, "width");
	pageWidth = (u32)std::stof(tag);
	tag = dmim::GetNextAttribute(svgFile, "height");
	pageHeight = (u32)std::stof(tag);
	
	// Layers
	while (tag != "g")
	{
		tag = dmim::GetNextTag(svgFile);
	}
	pRootLayer->PopulateFromFile(svgFile);
}

VectorArt& VectorArt::operator=(VectorArt&& other)
{
	pRootLayer = std::move(other.pRootLayer);
	other.pRootLayer = nullptr;
	pageWidth = other.pageWidth;
	pageHeight = other.pageHeight;
	return *this;
}

void VectorArt::Serialize(u32* pBuffer)
{
	*pBuffer = pageWidth;
	++pBuffer;

	*pBuffer = pageHeight;
	++pBuffer;

	pRootLayer->Serialize(pBuffer);
}
