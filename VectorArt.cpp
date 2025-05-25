#include "pch.h"

#include "VectorArt.h"

#include "DomImport/XMLUtils.h"

#include <fstream>
#include <string>

VectorArt::VectorArt(const char* pFilePath)
{
	if (!std::filesystem::exists(pFilePath))
	{
		DOMLOG_ERROR("Cannot find file: ", pFilePath);
		return;
	}
	
	std::ifstream svgFile(pFilePath, std::ios::binary); // We need to open in binary mode otherwise Tellg()/Seekg() will not work properly on Windows. (fuck microsoft)
	
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

	// #TODO: Empty layers cause a freeze
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

	if (pRootLayer)
	{
		pRootLayer->Serialize(pBuffer);
	}
}

VectorPrimitiveLayer* VectorArt::FindLayerByLabel(const std::string& label)
{
	return pRootLayer->FindLayerByLabel(label); 
}
