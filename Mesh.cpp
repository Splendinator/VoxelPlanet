#include "pch.h"
//
//#include "Mesh.h"
//#include "tinyobj_loader.h"
//
//std::unordered_map<std::string, Mesh*> meshMap; // Map of meshes that were created from their fileName
//
//Mesh::~Mesh()
//{
//	if (numVertices > 0)
//	{
//		delete[] vertexData;
//	}
//}
//
///*static*/ Mesh* Mesh::CreateOrGetMesh(const char* fileName)
//{
//	std::string fullFilePath = std::string("..\\Content\\Meshes\\") + std::string(fileName);
//
//	// Check for duplicates
//	auto foundMesh = meshMap.find(fullFilePath);
//	if (foundMesh != meshMap.end())
//	{
//		return foundMesh->second;
//	}
//
//	// Load a 3d model from file
//	tinyobj::attrib_t attributes;
//	std::vector<tinyobj::shape_t> shapes;
//	std::vector<tinyobj::material_t> materials;
//	std::string error;
//	std::string warning;
//
//	const bool bSuccess = tinyobj::LoadObj(&attributes, &shapes, &materials, &warning, &error, fullFilePath.c_str());
//
//	if (bSuccess)
//	{
//		if(shapes.size() == 1)
//		{
//			const tinyobj::shape_t & shape = shapes[0];
//
//			Mesh* newMesh = new Mesh;
//			newMesh->numVertices = (u32)shape.mesh.indices.size();
//			newMesh->bHasNormals = attributes.normals.size() > 0;
//			newMesh->bHasTexCoords = attributes.texcoords.size() > 0;
//
//			newMesh->vertexData = new float[newMesh->GetNumBytes()];
//
//			const u32 arrayIndexOffsetOfSingleVertex =
//				3 +
//				(newMesh->bHasNormals ? 3 : 0) +
//				(newMesh->bHasTexCoords ? 2 : 0);
//
//
//			for(u32 i = 0; i < newMesh->numVertices; ++i)
//			{
//				const tinyobj::index_t& index = shape.mesh.indices[i];
//
//				const u32 arrayIndexStartPos = i * arrayIndexOffsetOfSingleVertex;
//				u32 arrayIndexOffset = 0;
//
//					// Position				  i * indexOffsetOfSingleVertex
//				newMesh->vertexData[arrayIndexStartPos + arrayIndexOffset++] = (float)attributes.vertices[index.vertex_index * 3 + 0];
//				newMesh->vertexData[arrayIndexStartPos + arrayIndexOffset++] = (float)attributes.vertices[index.vertex_index * 3 + 1];
//				newMesh->vertexData[arrayIndexStartPos + arrayIndexOffset++] = (float)attributes.vertices[index.vertex_index * 3 + 2];
//
//				// Normals
//				if (newMesh->bHasNormals)
//				{
//					newMesh->vertexData[arrayIndexStartPos + arrayIndexOffset++] = (float)attributes.normals[index.normal_index * 3 + 0];
//					newMesh->vertexData[arrayIndexStartPos + arrayIndexOffset++] = (float)attributes.normals[index.normal_index * 3 + 1];
//					newMesh->vertexData[arrayIndexStartPos + arrayIndexOffset++] = (float)attributes.normals[index.normal_index * 3 + 2];
//				}
//
//				if (newMesh->bHasTexCoords)
//				{
//					// TexCoords
//					newMesh->vertexData[arrayIndexStartPos + arrayIndexOffset++] = (float)attributes.texcoords[index.texcoord_index * 2 + 0];
//					newMesh->vertexData[arrayIndexStartPos + arrayIndexOffset++] = (float)attributes.texcoords[index.texcoord_index * 2 + 1];
//				}													 
//			}
//
//			meshMap.emplace(fullFilePath, newMesh);
//			return newMesh;
//		}
//		else
//		{
//			DOMLOG_WARN("Expecting exactly one shape but got", shapes.size());
//		}
//	}
//	else
//	{
//		DOMLOG_WARN("No 3d model '", fullFilePath, "found!");
//	}
//	
//	return nullptr;
//}
//
//u32 Mesh::GetSingleVertexBytes() const
//{
//	int sizeOfSingleVertex = sizeof(float) * 3;
//
//	if (HasNormals())
//	{
//		sizeOfSingleVertex += sizeof(float) * 3;
//	}
//
//	if (HasTexCoords())
//	{
//		sizeOfSingleVertex += sizeof(float) * 2;
//	}
//
//	return sizeOfSingleVertex;
//}
