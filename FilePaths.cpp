#include "pch.h"

#include "FilePaths.h"

#define VECTOR_ART_DIRECTORY(fileName) "C:/Users/Dominic/Desktop/"fileName".svg";

namespace FilePath
{
	namespace VectorArt
	{
		const char* grass = VECTOR_ART_DIRECTORY("Grass");
		const char* tree = VECTOR_ART_DIRECTORY("Tree");
		const char* healthBar = VECTOR_ART_DIRECTORY("HealthBar");
		const char* player = VECTOR_ART_DIRECTORY("Player");
		const char* enemy = VECTOR_ART_DIRECTORY("Enemy");
		const char* dirt = VECTOR_ART_DIRECTORY("Dirt");
	}
}