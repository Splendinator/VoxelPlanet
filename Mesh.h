/// Not used in vector art renderer
//#pragma once
//
//#include "DomMath/Vec3.h"
//
//#include <vector>
//
//class Mesh
//{
//public:
//
//	~Mesh();
//
//	/// TEMP: Who should manage lifetime?
//	/// Renderer just deletes all meshes at the end maybe?
//	static Mesh* CreateOrGetMesh(const char* fileName);
//
//	u32 GetNumVertices() const { return numVertices; };
//	u32 GetNumBytes() const { return GetSingleVertexBytes() * numVertices; };
//	u32 GetSingleVertexBytes() const;
//	bool HasNormals() const { return bHasNormals; }
//	bool HasTexCoords() const { return bHasTexCoords; }
//	float* GetRawData() const { return vertexData; }
//
//protected:
//
//	float* vertexData; // Vertex data stored in the format (pos, [normals], [texCoords])
//
//	u32 numVertices = 0;
//	bool bHasNormals = false;
//	bool bHasTexCoords = false;
//};
//
