#include "pch.h"

#include "ResourceLoader.h"

#include "DomMath/Mat4.h"

#include "Mesh.h"
#include "Texture.h"
#include "Renderer.h"

void ResourceLoader::LoadResources()
{
	Mesh* monkeyMesh = Mesh::CreateOrGetMesh("Monkey.obj");
	Mesh* cubeMesh = Mesh::CreateOrGetMesh("Cube.obj");
	Texture* brickWallTexture = Texture::CreateOrGetTexture("BrickWall.png");
	Texture* greenTexture = Texture::CreateOrGetTexture("Green.png");

	void* pHandle = dmgf::AddNewObject(greenTexture, monkeyMesh);
	void* pHandle2 = dmgf::AddNewObject(brickWallTexture, cubeMesh);

	dmgf::SetObjectPos(pHandle, Mat4f::getTranslation({ 0.f, 0.f, -10.f }));
	dmgf::SetObjectPos(pHandle2, Mat4f::getTranslation({ 5.f, 0.f, -10.f }));
}
