#include "pch.h"

#include "Game.h"

#include "DomGraphics.h"

#include "DomLog/DomLog.h"
#include "DomMath/Vec3.h"
#include "DomMath/Constants.h"
#include "DomMath/Constexpr.h"
#include "DomMath/Math.h"
#include "DomUtils/DomUtils.h"

#include <imgui.h>

#ifdef PLANET_GEN

enum EQuadDir
{
	POSY,
	NEGY,
	POSX,
	NEGX,
	POSZ,
	NEGZ
};

// beginScale and endScale mean the scale of the thingy base off it's rotation around the planet (makes the squares turn to more triangular)
// radiusScale is the scale applied due to distance from the center of the planet
// beginHorizontalScale is the scale applied to NEGX, it is the starting scale of a block on this ring.
// endHorizontalScale is the scale applied to POSX, it is the ending scale of a block on this ring.
dmgf::Quad Generate(const Vec3f& position, EQuadDir dir, const Mat4f& rot, float beginHorizontalScale, float endHorizontalScale, float radiusScale, Vec3f colour)
{
	switch (dir)
	{
	case EQuadDir::POSZ:
	{
		const float begs = beginHorizontalScale;
		const float ends = endHorizontalScale;

		Vec3f pos1 = position + (rot * Vec4f(-0.5f, -0.5f, -0.5f + begs, 1.0f)).XYZ() * radiusScale;
		Vec3f pos2 = position + (rot * Vec4f(-0.5f, 0.5f, -0.5f + begs, 1.0f)).XYZ() * radiusScale;
		Vec3f pos3 = position + (rot * Vec4f(0.5f, 0.5f, -0.5f + ends, 1.0f)).XYZ() * radiusScale;
		Vec3f pos4 = position + (rot * Vec4f(-0.5f, -0.5f, -0.5f + begs, 1.0f)).XYZ() * radiusScale;
		Vec3f pos5 = position + (rot * Vec4f(0.5f, 0.5f, -0.5f + ends, 1.0f)).XYZ() * radiusScale;
		Vec3f pos6 = position + (rot * Vec4f(0.5f, -0.5f, -0.5f + ends, 1.0f)).XYZ() * radiusScale;

		Vec3f normal = (rot * Vec4f(0.0f, 0.0f, 1.0f, 1.0f)).XYZ();
		return
		{
			pos1.x, pos1.y, pos1.z, normal.x, normal.y, normal.z, colour.x, colour.y, colour.z,
			pos2.x, pos2.y, pos2.z, normal.x, normal.y, normal.z, colour.x, colour.y, colour.z,
			pos3.x, pos3.y, pos3.z, normal.x, normal.y, normal.z, colour.x, colour.y, colour.z,
			pos4.x, pos4.y, pos4.z, normal.x, normal.y, normal.z, colour.x, colour.y, colour.z,
			pos5.x, pos5.y, pos5.z, normal.x, normal.y, normal.z, colour.x, colour.y, colour.z,
			pos6.x, pos6.y, pos6.z, normal.x, normal.y, normal.z, colour.x, colour.y, colour.z
		};
	}
	case EQuadDir::NEGZ:
	{
		Vec3f pos1 = position + (rot * Vec4f(-0.5f, -0.5f, -0.5f, 1.0f)).XYZ() * radiusScale;
		Vec3f pos2 = position + (rot * Vec4f(-0.5f, 0.5f, -0.5f, 1.0f)).XYZ() * radiusScale;
		Vec3f pos3 = position + (rot * Vec4f(0.5f, 0.5f, -0.5f, 1.0f)).XYZ() * radiusScale;
		Vec3f pos4 = position + (rot * Vec4f(-0.5f, -0.5f, -0.5f, 1.0f)).XYZ() * radiusScale;
		Vec3f pos5 = position + (rot * Vec4f(0.5f, 0.5f, -0.5f, 1.0f)).XYZ() * radiusScale;
		Vec3f pos6 = position + (rot * Vec4f(0.5f, -0.5f, -0.5f, 1.0f)).XYZ() * radiusScale;

		Vec3f normal = (rot * Vec4f(0.0f, 0.0f, -1.0f, 1.0f)).XYZ();

		return
		{
			pos1.x, pos1.y, pos1.z, normal.x, normal.y, normal.z, colour.x, colour.y, colour.z,
			pos2.x, pos2.y, pos2.z, normal.x, normal.y, normal.z, colour.x, colour.y, colour.z,
			pos3.x, pos3.y, pos3.z, normal.x, normal.y, normal.z, colour.x, colour.y, colour.z,
			pos4.x, pos4.y, pos4.z, normal.x, normal.y, normal.z, colour.x, colour.y, colour.z,
			pos5.x, pos5.y, pos5.z, normal.x, normal.y, normal.z, colour.x, colour.y, colour.z,
			pos6.x, pos6.y, pos6.z, normal.x, normal.y, normal.z, colour.x, colour.y, colour.z
		};
	}
	case EQuadDir::POSX:
	{
		const float size = endHorizontalScale;

		Vec3f pos1 = position + (rot * Vec4f(0.5f, -0.5f, -0.5f + 0.0f, 1.0f)).XYZ() * radiusScale;
		Vec3f pos2 = position + (rot * Vec4f(0.5f, 0.5f, -0.5f + 0.0f, 1.0f)).XYZ() * radiusScale;
		Vec3f pos3 = position + (rot * Vec4f(0.5f, 0.5f, -0.5f + size, 1.0f)).XYZ() * radiusScale;
		Vec3f pos4 = position + (rot * Vec4f(0.5f, -0.5f, -0.5f + 0.0f, 1.0f)).XYZ() * radiusScale;
		Vec3f pos5 = position + (rot * Vec4f(0.5f, 0.5f, -0.5f + size, 1.0f)).XYZ() * radiusScale;
		Vec3f pos6 = position + (rot * Vec4f(0.5f, -0.5f, -0.5f + size, 1.0f)).XYZ() * radiusScale;

		Vec3f normal = (rot * Vec4f(1.0f, 0.0f, 0.0f, 1.0f)).XYZ();

		return
		{
			pos1.x, pos1.y, pos1.z, normal.x, normal.y, normal.z, colour.x, colour.y, colour.z,
			pos2.x, pos2.y, pos2.z, normal.x, normal.y, normal.z, colour.x, colour.y, colour.z,
			pos3.x, pos3.y, pos3.z, normal.x, normal.y, normal.z, colour.x, colour.y, colour.z,
			pos4.x, pos4.y, pos4.z, normal.x, normal.y, normal.z, colour.x, colour.y, colour.z,
			pos5.x, pos5.y, pos5.z, normal.x, normal.y, normal.z, colour.x, colour.y, colour.z,
			pos6.x, pos6.y, pos6.z, normal.x, normal.y, normal.z, colour.x, colour.y, colour.z
		};
	}
	case EQuadDir::NEGX:
	{
		const float size = beginHorizontalScale;

		Vec3f pos1 = position + (rot * Vec4f(-0.5f, -0.5f, -0.5f + 0.0f, 1.0f)).XYZ() * radiusScale;
		Vec3f pos2 = position + (rot * Vec4f(-0.5f, 0.5f, -0.5f + 0.0f, 1.0f)).XYZ() * radiusScale;
		Vec3f pos3 = position + (rot * Vec4f(-0.5f, 0.5f, -0.5f + size, 1.0f)).XYZ() * radiusScale;
		Vec3f pos4 = position + (rot * Vec4f(-0.5f, -0.5f, -0.5f + 0.0f, 1.0f)).XYZ() * radiusScale;
		Vec3f pos5 = position + (rot * Vec4f(-0.5f, 0.5f, -0.5f + size, 1.0f)).XYZ() * radiusScale;
		Vec3f pos6 = position + (rot * Vec4f(-0.5f, -0.5f, -0.5f + size, 1.0f)).XYZ() * radiusScale;

		Vec3f normal = (rot * Vec4f(-1.0f, 0.0f, 0.0f, 1.0f)).XYZ();

		return
		{
			pos1.x, pos1.y, pos1.z, normal.x, normal.y, normal.z, colour.x, colour.y, colour.z,
			pos2.x, pos2.y, pos2.z, normal.x, normal.y, normal.z, colour.x, colour.y, colour.z,
			pos3.x, pos3.y, pos3.z, normal.x, normal.y, normal.z, colour.x, colour.y, colour.z,
			pos4.x, pos4.y, pos4.z, normal.x, normal.y, normal.z, colour.x, colour.y, colour.z,
			pos5.x, pos5.y, pos5.z, normal.x, normal.y, normal.z, colour.x, colour.y, colour.z,
			pos6.x, pos6.y, pos6.z, normal.x, normal.y, normal.z, colour.x, colour.y, colour.z
		};
	}
	case EQuadDir::POSY:
	{
		Vec3f pos1 = position + (rot * Vec4f(-0.5f, 0.5f, -0.5f + 0.0f * beginHorizontalScale, 1.0f)).XYZ() * radiusScale;
		Vec3f pos2 = position + (rot * Vec4f(-0.5f, 0.5f, -0.5f + 1.0f * beginHorizontalScale, 1.0f)).XYZ() * radiusScale;
		Vec3f pos3 = position + (rot * Vec4f(0.5f, 0.5f, -0.5f + 1.0f * endHorizontalScale, 1.0f)).XYZ() * radiusScale;
		Vec3f pos4 = position + (rot * Vec4f(-0.5f, 0.5f, -0.5f + 0.0f * beginHorizontalScale, 1.0f)).XYZ() * radiusScale;
		Vec3f pos5 = position + (rot * Vec4f(0.5f, 0.5f, -0.5f + 1.0f * endHorizontalScale, 1.0f)).XYZ() * radiusScale;
		Vec3f pos6 = position + (rot * Vec4f(0.5f, 0.5f, -0.5f + 0.0f * endHorizontalScale, 1.0f)).XYZ() * radiusScale;

		Vec3f normal = (rot * Vec4f(0.0f, 1.0f, 0.0f, 1.0f)).XYZ();

		return
		{
			pos1.x, pos1.y, pos1.z, normal.x, normal.y, normal.z, colour.x, colour.y, colour.z,
			pos2.x, pos2.y, pos2.z, normal.x, normal.y, normal.z, colour.x, colour.y, colour.z,
			pos3.x, pos3.y, pos3.z, normal.x, normal.y, normal.z, colour.x, colour.y, colour.z,
			pos4.x, pos4.y, pos4.z, normal.x, normal.y, normal.z, colour.x, colour.y, colour.z,
			pos5.x, pos5.y, pos5.z, normal.x, normal.y, normal.z, colour.x, colour.y, colour.z,
			pos6.x, pos6.y, pos6.z, normal.x, normal.y, normal.z, colour.x, colour.y, colour.z
		};
	}
	case EQuadDir::NEGY:
	{
		Vec3f pos1 = position + (rot * Vec4f(-0.5f, -0.5f, -0.5f + 0.0f * beginHorizontalScale, 1.0f)).XYZ() * radiusScale;
		Vec3f pos2 = position + (rot * Vec4f(-0.5f, -0.5f, -0.5f + 1.0f * beginHorizontalScale, 1.0f)).XYZ() * radiusScale;
		Vec3f pos3 = position + (rot * Vec4f(0.5f, -0.5f, -0.5f + 1.0f * endHorizontalScale, 1.0f)).XYZ() * radiusScale;
		Vec3f pos4 = position + (rot * Vec4f(-0.5f, -0.5f, -0.5f + 0.0f * beginHorizontalScale, 1.0f)).XYZ() * radiusScale;
		Vec3f pos5 = position + (rot * Vec4f(0.5f, -0.5f, -0.5f + 1.0f * endHorizontalScale, 1.0f)).XYZ() * radiusScale;
		Vec3f pos6 = position + (rot * Vec4f(0.5f, -0.5f, -0.5f + 0.0f * endHorizontalScale, 1.0f)).XYZ() * radiusScale;

		Vec3f normal = (rot * Vec4f(0.0f, -1.0f, 0.0f, 1.0f)).XYZ();

		return
		{
			pos1.x, pos1.y, pos1.z, normal.x, normal.y, normal.z, colour.x, colour.y, colour.z,
			pos2.x, pos2.y, pos2.z, normal.x, normal.y, normal.z, colour.x, colour.y, colour.z,
			pos3.x, pos3.y, pos3.z, normal.x, normal.y, normal.z, colour.x, colour.y, colour.z,
			pos4.x, pos4.y, pos4.z, normal.x, normal.y, normal.z, colour.x, colour.y, colour.z,
			pos5.x, pos5.y, pos5.z, normal.x, normal.y, normal.z, colour.x, colour.y, colour.z,
			pos6.x, pos6.y, pos6.z, normal.x, normal.y, normal.z, colour.x, colour.y, colour.z
		};
	}
	}

	DOMLOG_WARN("Unsupported quad direction", dir);
	return {};
}

constexpr int CUBES_PER_RING = 512; //(int)(2.0f * PLANET_RADIUS * dmma::piFloat);
constexpr float PLANET_RADIUS = (float)CUBES_PER_RING / (dmma::piFloat * 2.0f);
constexpr float ROTATION_PER_CUBE = (2.0f * dmma::piFloat) / (float)CUBES_PER_RING;

Vec3f GenerateColour(float height, Vec3f dir)
{
	struct HeightEntry
	{
		Vec3f colour;
		float height;
	};

	HeightEntry heightColours[] =
	{
		{{1.0f, 1.0f, 1.0f}, {30.f}}, // Snow
		{{0.5f, 0.5f, 0.5f}, {27.f}}, // Rock
		{{0.0f, 0.42f, 0.14f}, {22.f}}, // Grass
		{{1.0f, 0.9f, 0.4f}, {19.f}}, // Sand
		{{0.0f, 0.21f, 0.96f}, {0.f}}, // Water
	};

	for (HeightEntry& heightColour : heightColours)
	{
		if (height >= heightColour.height)
		{
			return heightColour.colour;
		}
	}

	return { 0.0f, 0.0f, 0.0f };
}

int GenerateHeight(Vec3f dir)
{
	return 25.f + sin(dir.x * 8.0f) * 3.2f + cos(dir.y * 13.f) * 1.5f + sin(dir.z * 12.f) * 2.3f;
}

void GenerateCube(Vec3f position, const Mat4f& rot, float beginScale, float endScale, float radiusScale, Vec3f colour)
{
	///dmgf::AddQuad(Generate(position, EQuadDir::NEGY, rot, beginScale, endScale, radiusScale, colour));
	dmgf::AddQuad(Generate(position, EQuadDir::POSY, rot, beginScale, endScale, radiusScale, colour));
	dmgf::AddQuad(Generate(position, EQuadDir::NEGX, rot, beginScale, endScale, radiusScale, colour));
	dmgf::AddQuad(Generate(position, EQuadDir::POSX, rot, beginScale, endScale, radiusScale, colour));
	dmgf::AddQuad(Generate(position, EQuadDir::NEGZ, rot, beginScale, endScale, radiusScale, colour));
	dmgf::AddQuad(Generate(position, EQuadDir::POSZ, rot, beginScale, endScale, radiusScale, colour));
}

void GeneratePlanetCube(const Vec3f& dir, Mat4f rot, bool bCheckered, float beginScale, float endScale)
{
	const float generatedHeight = (float)GenerateHeight(dir);

	const Vec3f pos = dir * (PLANET_RADIUS + GenerateHeight(dir));

	// #TODO_OLD: This should scale between radiusScaleStart and radiusScaleEnd
	const float radiusScale = (generatedHeight + 1.0f + PLANET_RADIUS) / PLANET_RADIUS;

	const Vec3f colour = GenerateColour(generatedHeight, dir);

	GenerateCube(pos, rot, beginScale, endScale, radiusScale, colour);
}

int FindMipLevelRecurse(int ring, int threshold, int thresholdAdd, int mipLevel)
{
	if (abs(ring) <= threshold)
	{
		return mipLevel;
	}
	else if (thresholdAdd >= 1)
	{
		return FindMipLevelRecurse(ring, threshold + thresholdAdd, thresholdAdd / 2, mipLevel * 2);
	}
	else
	{
		return CUBES_PER_RING;
	}
}

int FindMipLevel(int ring)
{
	static constexpr int startNum = CUBES_PER_RING / 8;

	return FindMipLevelRecurse(ring, startNum, startNum / 2, 1);
}

void game::init(int height, int width)
{
	Vec4f startDirY = { 0, 1.0f, 0, 1.0f };
	Vec4f startDirX = { 1.0f, 0.0f, 0, 1.0f };

	bool bCheckered = false;

	for (int ring = (-CUBES_PER_RING / 4); ring <= CUBES_PER_RING / 4; ring++)
	{
		Mat4f ringRotationMatrixZ = dmma::generateRotationZ(ROTATION_PER_CUBE * ring);

		const int cubesInThisRing = CUBES_PER_RING / FindMipLevel(ring);
		if (!(cubesInThisRing % 2))
		{
			bCheckered = !bCheckered;
		}

		float beginScale = 1.0f;
		float endScale = 1.0f;

		if (abs(ring) != CUBES_PER_RING / 4)
		{
			beginScale = cos(ROTATION_PER_CUBE * (ring - 0.5f)) * FindMipLevel(ring);
			endScale = cos(ROTATION_PER_CUBE * (ring + 0.5f)) * FindMipLevel(ring);
		}

		for (int i = 0; i < cubesInThisRing; ++i)
		{
			const float rotationPerCube = (2.0f * dmma::piFloat) / (float)cubesInThisRing;

			Mat4f rotationMatrixX = dmma::generateRotationX(rotationPerCube * i) * ringRotationMatrixZ;

			bCheckered = !bCheckered;

			GeneratePlanetCube((rotationMatrixX * startDirY).XYZ(), rotationMatrixX, bCheckered, beginScale, endScale);
		}
	}
}

void game::tick(float deltaTime)
{
	static int numFrames = 0;
	static float totalTime = 0.0f;
	static int fps = 0;

	totalTime += deltaTime;
	++numFrames;

	if (totalTime > 1.0f)
	{
		fps = numFrames;
		numFrames = 0;
		totalTime -= 1.0f;
	}

	ImGui::SetWindowPos({ 0,0 }, ImGuiCond_FirstUseEver);
	ImGui::SetWindowSize({ 200,200 }, ImGuiCond_FirstUseEver);
	ImGui::Begin("FPS");
	ImGui::Text("Frame rate: %d", fps);
	ImGui::End();
}

#endif //~ #ifdef PLANET_GEN