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

	// Distance between grid points, smaller numbers mean sharper noise.
	// The noise will always return 0.0f on grid co-ordinates.
	EDITORPROPERTY()
	float gridSize = 1.0f;

	// Overall multipler to the output such that we always output a number between (-magnitude, +magnitude)
	EDITORPROPERTY()
	float magnitude = 1.0f;

	// Optional modulus to cause grid co-ordinates to wrap around. Useful if you want the noise to "join up" again.
	// We use this modulud when generating the shoreline of out island so the noise "joins up" again after the full circle
	EDITORPROPERTY()
	float gridModulus = -1.0f;
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

	// The basic noise algorithm works by picking nearby grid points, giving them a gradient (-0.5f, +0.5f) and then picking points
	// based off the surrounding grid points' gradients. It returns a value between (-magnitude, +magnitude)
	float BasicNoise1D(float x, RandSeed seed, BasicNoiseParams params);
}