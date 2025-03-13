#pragma once

union RandSeed
{
	u64 seed;
	u32 half[2];
	u16 quarter[2];
	u8	byte[8];
};

EDITORSTRUCT()
struct BasicNoiseParams
{
	EDITORBODY()

	EDITORPROPERTY()
	float gridSize = 1.0f;

	EDITORPROPERTY()
	float magnitude = 1.0f;
};

// Perlin noise etc.
namespace WorldGenerationUtils
{
	// Used for debugging only
	void Test(RandSeed randSeed);

	void MutateSeed(RandSeed& r);
	
	RandSeed ConstructSeedFromBytes(u8 byte1, u8 byte2, u8 byte3, u8 byte4, u8 byte5, u8 byte6, u8 byte7, u8 byte8);
	
	// Between 0.0f and 1.0f
	float RandFloat(RandSeed randSeed);

	// Creates a basic, sort of sharp noise. Return between -magnitude and +magnitude.
	float BasicNoise1D(float x, RandSeed seed, BasicNoiseParams params);
}