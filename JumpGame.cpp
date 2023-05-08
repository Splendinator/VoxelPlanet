#include "pch.h"

#ifdef JUMP_GAME /// We aren't using this file any more so IFDEF it out

#include "Game.h"

#include "../Graphics/DomGraphics.h"

#include "DomLog/DomLog.h"
#include "DomMath/Vec3.h"
#include "DomMath/Constants.h"
#include "DomMath/Constexpr.h"
#include "DomMath/Math.h"
#include "DomUtils/DomUtils.h"
#include "DomWindow/DomWindow.h"
#include "../Graphics/Camera.h"

#include <imgui.h>

enum EQuadDir
{
	POSY,
	NEGY,
	POSX,
	NEGX,
	POSZ,
	NEGZ
};

dmgf::Quad GenerateQuad(const Vec3f& position, EQuadDir dir, Vec3f colour)
{
	switch (dir)
	{
	case EQuadDir::POSZ:
	{
		const Vec3f pos1 = position + Vec3f(-0.5f, -0.5f, 0.5f);
		const Vec3f pos2 = position + Vec3f(-0.5f,  0.5f, 0.5f);
		const Vec3f pos3 = position + Vec3f( 0.5f,  0.5f, 0.5f);
		const Vec3f pos4 = position + Vec3f(-0.5f, -0.5f, 0.5f);
		const Vec3f pos5 = position + Vec3f( 0.5f,  0.5f, 0.5f);
		const Vec3f pos6 = position + Vec3f( 0.5f, -0.5f, 0.5f);
		const Vec3f normal(0.0f, 0.0f, 1.0f);

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
		const Vec3f pos1 = position + Vec3f(-0.5f, -0.5f, -0.5f);
		const Vec3f pos2 = position + Vec3f(-0.5f,  0.5f, -0.5f);
		const Vec3f pos3 = position + Vec3f( 0.5f,  0.5f, -0.5f);
		const Vec3f pos4 = position + Vec3f(-0.5f, -0.5f, -0.5f);
		const Vec3f pos5 = position + Vec3f( 0.5f,  0.5f, -0.5f);
		const Vec3f pos6 = position + Vec3f( 0.5f, -0.5f, -0.5f);
		const Vec3f normal(0.0f, 0.0f, -1.0f);

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
		const Vec3f pos1 = position + Vec3f( 0.5f, -0.5f, -0.5f);
		const Vec3f pos2 = position + Vec3f( 0.5f,  0.5f, -0.5f);
		const Vec3f pos3 = position + Vec3f( 0.5f,  0.5f,  0.5f);
		const Vec3f pos4 = position + Vec3f( 0.5f, -0.5f, -0.5f);
		const Vec3f pos5 = position + Vec3f( 0.5f,  0.5f,  0.5f);
		const Vec3f pos6 = position + Vec3f( 0.5f, -0.5f,  0.5f);
		const Vec3f normal(1.0f, 0.0f, 0.0f);

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
		const Vec3f pos1 = position + Vec3f(-0.5f, -0.5f, -0.5f);
		const Vec3f pos2 = position + Vec3f(-0.5f,  0.5f, -0.5f);
		const Vec3f pos3 = position + Vec3f(-0.5f,  0.5f,  0.5f);
		const Vec3f pos4 = position + Vec3f(-0.5f, -0.5f, -0.5f);
		const Vec3f pos5 = position + Vec3f(-0.5f,  0.5f,  0.5f);
		const Vec3f pos6 = position + Vec3f(-0.5f, -0.5f,  0.5f);
		const Vec3f normal(-1.0f, 0.0f, 0.0f);

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
		const Vec3f pos1 = position + Vec3f(-0.5f, 0.5f, -0.5f);
		const Vec3f pos2 = position + Vec3f(-0.5f, 0.5f,  0.5f);
		const Vec3f pos3 = position + Vec3f( 0.5f, 0.5f,  0.5f);
		const Vec3f pos4 = position + Vec3f(-0.5f, 0.5f, -0.5f);
		const Vec3f pos5 = position + Vec3f( 0.5f, 0.5f,  0.5f);
		const Vec3f pos6 = position + Vec3f( 0.5f, 0.5f, -0.5f);
		const Vec3f normal(0.0f, 1.0f, 0.0f);

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
		const Vec3f pos1 = position + Vec3f(-0.5f, -0.5f, -0.5f);
		const Vec3f pos2 = position + Vec3f(-0.5f, -0.5f,  0.5f);
		const Vec3f pos3 = position + Vec3f( 0.5f, -0.5f,  0.5f);
		const Vec3f pos4 = position + Vec3f(-0.5f, -0.5f, -0.5f);
		const Vec3f pos5 = position + Vec3f( 0.5f, -0.5f,  0.5f);
		const Vec3f pos6 = position + Vec3f( 0.5f, -0.5f, -0.5f);
		const Vec3f normal = Vec3f(0.0f, -1.0f, 0.0f);

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

void GenerateCube(Vec3f position, Vec3f colour)
{
	dmgf::AddQuad(GenerateQuad(position, EQuadDir::NEGY, colour));
	dmgf::AddQuad(GenerateQuad(position, EQuadDir::POSY, colour));
	dmgf::AddQuad(GenerateQuad(position, EQuadDir::NEGX, colour));
	dmgf::AddQuad(GenerateQuad(position, EQuadDir::POSX, colour));
	dmgf::AddQuad(GenerateQuad(position, EQuadDir::NEGZ, colour));
	dmgf::AddQuad(GenerateQuad(position, EQuadDir::POSZ, colour));
}

const int CUBE_SIZE = 100;
const int PLATFORM_RARITY = 100;

bool blockGrid[CUBE_SIZE * CUBE_SIZE * CUBE_SIZE] = {};

bool IsInBlockGrid(int x, int y, int z)
{
	if (x < 0 || y < 0 || z < 0 || x >= CUBE_SIZE || y >= CUBE_SIZE || z >= CUBE_SIZE)
	{
		return false;
	}
	else
	{
		return blockGrid[x + CUBE_SIZE * y + CUBE_SIZE * CUBE_SIZE * z];
	}
}

#include <time.h>
void Game::init(int height, int width)
{
	srand(time(NULL));
	for (int i = 0; i < CUBE_SIZE; ++i)
	{
		for (int j = 0; j < CUBE_SIZE; ++j)
		{
			for (int k = 0; k < CUBE_SIZE; ++k)
			{
				if (!(i % (CUBE_SIZE-1)) || (j == 0) || !(k % (CUBE_SIZE - 1)) || !(rand() % PLATFORM_RARITY))
				{
					blockGrid[i + CUBE_SIZE * j + CUBE_SIZE * CUBE_SIZE * k] = true;
					GenerateCube({ (float)i, (float)j, (float)k }, { i % 10 / 10.f, j % 10 / 10.f, k % 10 / 10.f});
				}
				else
				{
					blockGrid[i + CUBE_SIZE * j + CUBE_SIZE * CUBE_SIZE * k] = false;
				}
			}
		}
	}
}

void CreateImGuiWindow(float deltaTime)
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

void GameplayTick(float deltaTime)
{
	constexpr float MOVE_SPEED = 40.0f;
	constexpr float MAX_SPEED = 20.f;
	constexpr float JUMP_VELOCITY = 12.f;
	const Vec3f GRAVITY_VECTOR = { 0.0f, -20.f, 0.0f };
	static bool bJumping = false;
	
	static bool bInGameplayMode = false;
	static Vec3f cameraVelocity = { 0.0f, 0.0f, 0.0f };

	ImGui::SetWindowPos({ 0,800 }, ImGuiCond_FirstUseEver);
	ImGui::SetWindowSize({ 200,200 }, ImGuiCond_FirstUseEver);
	ImGui::Begin("Game");
	ImGui::Text("bJumping: %s", bJumping ? "true" : "false");
	ImGui::DragFloat3("Velocity", (float*)&cameraVelocity);

	if (dmwi::IsCursorLocked())
	{
		Camera& camera = dmgf::GetCamera();
		Vec3f& cameraPosF = camera.GetPos();
		if (dmwi::isPressed(dmwi::Button::F2))
		{
			bInGameplayMode = !bInGameplayMode;	

			camera.SetCameraShouldTick(!bInGameplayMode);

			if (bInGameplayMode)
			{
				bJumping = false;

				cameraVelocity = { 0.f,0.f,0.f };
			}
		}

		if (bInGameplayMode)
		{	
			cameraPosF += cameraVelocity * deltaTime;

			cameraVelocity += GRAVITY_VECTOR * deltaTime;

			if (dmwi::isHeld(dmwi::Button::W))
			{
				cameraVelocity += camera.ForwardNoPitch() * MOVE_SPEED * deltaTime;
			}
			if (dmwi::isHeld(dmwi::Button::S))
			{
				cameraVelocity += camera.BackNoPitch() * MOVE_SPEED * deltaTime;
			}
			if (dmwi::isHeld(dmwi::Button::A))
			{
				cameraVelocity += camera.LeftNoPitch() * MOVE_SPEED * deltaTime;
			}
			if (dmwi::isHeld(dmwi::Button::D))
			{
				cameraVelocity += camera.RightNoPitch() * MOVE_SPEED * deltaTime;
			}
			const float horizSpeed = sqrt(cameraVelocity.x * cameraVelocity.x + cameraVelocity.z * cameraVelocity.z);
			if (horizSpeed > MAX_SPEED)
			{
				const float speedDampenMult = MAX_SPEED / horizSpeed;
				
				cameraVelocity.x *= speedDampenMult;
				cameraVelocity.z *= speedDampenMult;
			}

			float speedDampMult = 1.0f - (7.0f * deltaTime);
			if (speedDampMult < 0.0f) speedDampMult = 0.0f;

			cameraVelocity.x *= speedDampMult;
			cameraVelocity.z *= speedDampMult;

			bool bInAir = false;
			if (IsInBlockGrid((int)(cameraPosF.x + 0.5f), (int)(cameraPosF.y - 1), (int)(cameraPosF.z + 0.5f)))
			{
				// Touching block				
				bInAir = true;
				
				if (cameraVelocity.y < 0.0f)
				{
					cameraVelocity.y = 0.0f;
					bJumping = false;
				}

				if (!bJumping && dmwi::isPressed(dmwi::SPACE))
				{
					bJumping = true;
					cameraVelocity.y = JUMP_VELOCITY;
				}
			}

			auto PushOutOfCube = [&cameraPosF](float xOffset, float yOffset, float zOffset)
			{
				if (IsInBlockGrid((int)(cameraPosF.x + xOffset), (int)(cameraPosF.y + yOffset), (int)(cameraPosF.z + zOffset)))
				{
					const int centerX = (int)cameraPosF.x + xOffset;
					const int centerY = (int)cameraPosF.y + yOffset;
					const int centerZ = (int)cameraPosF.z + zOffset;

					const float distX = cameraPosF.x - 0.5f - (float)centerX;
					const float distY = cameraPosF.y - 0.5f - (float)centerY;
					const float distZ = cameraPosF.z - 0.5f - (float)centerZ;

					if (abs(distX) < abs(distZ))
					{
						if (abs(distY) < abs(distX))
						{
							cameraPosF.y -= distY;
						}
						else
						{
							cameraPosF.x -= distX;
						}
					}
					else
					{
						if (abs(distY) < abs(distZ))
						{
							cameraPosF.y -= distY;
						}
						else
						{
							cameraPosF.z -= distZ;
						}
					}
				}
			};

			///PushOutOfCube(-0.5f, -0.5f, 0.5f);
			///PushOutOfCube(-0.5f, 0.5f, 0.5f);
			///PushOutOfCube(-0.5f, -0.5f, -0.5f);
			///PushOutOfCube(-0.5f, 0.5f, -0.5f);
			///PushOutOfCube(-0.5f, -0.5f, 1.5f);
			///PushOutOfCube(-0.5f, 0.5f, 1.5f);

			PushOutOfCube(0.5f, 0.5f, 0.5f);
			PushOutOfCube(0.5f, -0.5f, 0.5f);
			///PushOutOfCube(0.5f, -0.5f, -0.5f);
			///PushOutOfCube(0.5f, 0.5f, -0.5f);
			///PushOutOfCube(0.5f, -0.5f, 1.5f);
			///PushOutOfCube(0.5f, 0.5f, 1.5f);
			///
			///PushOutOfCube(1.5f, -0.5f, 0.5f);
			///PushOutOfCube(1.5f, 0.5f, 0.5f);
			///PushOutOfCube(1.5f, -0.5f, -0.5f);
			///PushOutOfCube(1.5f, 0.5f, -0.5f);
			///PushOutOfCube(1.5f, -0.5f, 1.5f);
			///PushOutOfCube(1.5f, 0.5f, 1.5f);
		}
	}

	ImGui::End();
}

void Game::Tick(float deltaTime)
{
	CreateImGuiWindow(deltaTime);
	GameplayTick(deltaTime);
}


#include "Game.h"

#include "../Graphics/DomGraphics.h"

#include "DomLog/DomLog.h"
#include "DomMath/Vec3.h"
#include "DomMath/Constants.h"
#include "DomMath/Constexpr.h"
#include "DomMath/Math.h"
#include "DomUtils/DomUtils.h"
#include "DomWindow/DomWindow.h"
#include "../Graphics/Camera.h"

#include <imgui.h>

enum EQuadDir
{
	POSY,
	NEGY,
	POSX,
	NEGX,
	POSZ,
	NEGZ
};

dmgf::Quad GenerateQuad(const Vec3f& position, EQuadDir dir, Vec3f colour)
{
	switch (dir)
	{
	case EQuadDir::POSZ:
	{
		const Vec3f pos1 = position + Vec3f(-0.5f, -0.5f, 0.5f);
		const Vec3f pos2 = position + Vec3f(-0.5f, 0.5f, 0.5f);
		const Vec3f pos3 = position + Vec3f(0.5f, 0.5f, 0.5f);
		const Vec3f pos4 = position + Vec3f(-0.5f, -0.5f, 0.5f);
		const Vec3f pos5 = position + Vec3f(0.5f, 0.5f, 0.5f);
		const Vec3f pos6 = position + Vec3f(0.5f, -0.5f, 0.5f);
		const Vec3f normal(0.0f, 0.0f, 1.0f);

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
		const Vec3f pos1 = position + Vec3f(-0.5f, -0.5f, -0.5f);
		const Vec3f pos2 = position + Vec3f(-0.5f, 0.5f, -0.5f);
		const Vec3f pos3 = position + Vec3f(0.5f, 0.5f, -0.5f);
		const Vec3f pos4 = position + Vec3f(-0.5f, -0.5f, -0.5f);
		const Vec3f pos5 = position + Vec3f(0.5f, 0.5f, -0.5f);
		const Vec3f pos6 = position + Vec3f(0.5f, -0.5f, -0.5f);
		const Vec3f normal(0.0f, 0.0f, -1.0f);

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
		const Vec3f pos1 = position + Vec3f(0.5f, -0.5f, -0.5f);
		const Vec3f pos2 = position + Vec3f(0.5f, 0.5f, -0.5f);
		const Vec3f pos3 = position + Vec3f(0.5f, 0.5f, 0.5f);
		const Vec3f pos4 = position + Vec3f(0.5f, -0.5f, -0.5f);
		const Vec3f pos5 = position + Vec3f(0.5f, 0.5f, 0.5f);
		const Vec3f pos6 = position + Vec3f(0.5f, -0.5f, 0.5f);
		const Vec3f normal(1.0f, 0.0f, 0.0f);

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
		const Vec3f pos1 = position + Vec3f(-0.5f, -0.5f, -0.5f);
		const Vec3f pos2 = position + Vec3f(-0.5f, 0.5f, -0.5f);
		const Vec3f pos3 = position + Vec3f(-0.5f, 0.5f, 0.5f);
		const Vec3f pos4 = position + Vec3f(-0.5f, -0.5f, -0.5f);
		const Vec3f pos5 = position + Vec3f(-0.5f, 0.5f, 0.5f);
		const Vec3f pos6 = position + Vec3f(-0.5f, -0.5f, 0.5f);
		const Vec3f normal(-1.0f, 0.0f, 0.0f);

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
		const Vec3f pos1 = position + Vec3f(-0.5f, 0.5f, -0.5f);
		const Vec3f pos2 = position + Vec3f(-0.5f, 0.5f, 0.5f);
		const Vec3f pos3 = position + Vec3f(0.5f, 0.5f, 0.5f);
		const Vec3f pos4 = position + Vec3f(-0.5f, 0.5f, -0.5f);
		const Vec3f pos5 = position + Vec3f(0.5f, 0.5f, 0.5f);
		const Vec3f pos6 = position + Vec3f(0.5f, 0.5f, -0.5f);
		const Vec3f normal(0.0f, 1.0f, 0.0f);

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
		const Vec3f pos1 = position + Vec3f(-0.5f, -0.5f, -0.5f);
		const Vec3f pos2 = position + Vec3f(-0.5f, -0.5f, 0.5f);
		const Vec3f pos3 = position + Vec3f(0.5f, -0.5f, 0.5f);
		const Vec3f pos4 = position + Vec3f(-0.5f, -0.5f, -0.5f);
		const Vec3f pos5 = position + Vec3f(0.5f, -0.5f, 0.5f);
		const Vec3f pos6 = position + Vec3f(0.5f, -0.5f, -0.5f);
		const Vec3f normal = Vec3f(0.0f, -1.0f, 0.0f);

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

void GenerateCube(Vec3f position, Vec3f colour)
{
	dmgf::AddQuad(GenerateQuad(position, EQuadDir::NEGY, colour));
	dmgf::AddQuad(GenerateQuad(position, EQuadDir::POSY, colour));
	dmgf::AddQuad(GenerateQuad(position, EQuadDir::NEGX, colour));
	dmgf::AddQuad(GenerateQuad(position, EQuadDir::POSX, colour));
	dmgf::AddQuad(GenerateQuad(position, EQuadDir::NEGZ, colour));
	dmgf::AddQuad(GenerateQuad(position, EQuadDir::POSZ, colour));
}

const int CUBE_SIZE = 100;
const int PLATFORM_RARITY = 100;

bool blockGrid[CUBE_SIZE * CUBE_SIZE * CUBE_SIZE] = {};

bool IsInBlockGrid(int x, int y, int z)
{
	if (x < 0 || y < 0 || z < 0 || x >= CUBE_SIZE || y >= CUBE_SIZE || z >= CUBE_SIZE)
	{
		return false;
	}
	else
	{
		return blockGrid[x + CUBE_SIZE * y + CUBE_SIZE * CUBE_SIZE * z];
	}
}

#include <time.h>
void Game::init(int height, int width)
{
	srand(time(NULL));
	for (int i = 0; i < CUBE_SIZE; ++i)
	{
		for (int j = 0; j < CUBE_SIZE; ++j)
		{
			for (int k = 0; k < CUBE_SIZE; ++k)
			{
				if (!(i % (CUBE_SIZE - 1)) || (j == 0) || !(k % (CUBE_SIZE - 1)) || !(rand() % PLATFORM_RARITY))
				{
					blockGrid[i + CUBE_SIZE * j + CUBE_SIZE * CUBE_SIZE * k] = true;
					GenerateCube({ (float)i, (float)j, (float)k }, { i % 10 / 10.f, j % 10 / 10.f, k % 10 / 10.f });
				}
				else
				{
					blockGrid[i + CUBE_SIZE * j + CUBE_SIZE * CUBE_SIZE * k] = false;
				}
			}
		}
	}
}

void CreateImGuiWindow(float deltaTime)
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

void GameplayTick(float deltaTime)
{
	constexpr float MOVE_SPEED = 40.0f;
	constexpr float MAX_SPEED = 20.f;
	constexpr float JUMP_VELOCITY = 12.f;
	const Vec3f GRAVITY_VECTOR = { 0.0f, -20.f, 0.0f };
	static bool bJumping = false;

	static bool bInGameplayMode = false;
	static Vec3f cameraVelocity = { 0.0f, 0.0f, 0.0f };

	ImGui::SetWindowPos({ 0,800 }, ImGuiCond_FirstUseEver);
	ImGui::SetWindowSize({ 200,200 }, ImGuiCond_FirstUseEver);
	ImGui::Begin("Game");
	ImGui::Text("bJumping: %s", bJumping ? "true" : "false");
	ImGui::DragFloat3("Velocity", (float*)&cameraVelocity);

	if (dmwi::IsCursorLocked())
	{
		Camera& camera = dmgf::GetCamera();
		Vec3f& cameraPosF = camera.GetPos();
		if (dmwi::isPressed(dmwi::Button::F2))
		{
			bInGameplayMode = !bInGameplayMode;

			camera.SetCameraShouldTick(!bInGameplayMode);

			if (bInGameplayMode)
			{
				bJumping = false;

				cameraVelocity = { 0.f,0.f,0.f };
			}
		}

		if (bInGameplayMode)
		{
			cameraPosF += cameraVelocity * deltaTime;

			cameraVelocity += GRAVITY_VECTOR * deltaTime;

			if (dmwi::isHeld(dmwi::Button::W))
			{
				cameraVelocity += camera.ForwardNoPitch() * MOVE_SPEED * deltaTime;
			}
			if (dmwi::isHeld(dmwi::Button::S))
			{
				cameraVelocity += camera.BackNoPitch() * MOVE_SPEED * deltaTime;
			}
			if (dmwi::isHeld(dmwi::Button::A))
			{
				cameraVelocity += camera.LeftNoPitch() * MOVE_SPEED * deltaTime;
			}
			if (dmwi::isHeld(dmwi::Button::D))
			{
				cameraVelocity += camera.RightNoPitch() * MOVE_SPEED * deltaTime;
			}
			const float horizSpeed = sqrt(cameraVelocity.x * cameraVelocity.x + cameraVelocity.z * cameraVelocity.z);
			if (horizSpeed > MAX_SPEED)
			{
				const float speedDampenMult = MAX_SPEED / horizSpeed;

				cameraVelocity.x *= speedDampenMult;
				cameraVelocity.z *= speedDampenMult;
			}

			float speedDampMult = 1.0f - (7.0f * deltaTime);
			if (speedDampMult < 0.0f) speedDampMult = 0.0f;

			cameraVelocity.x *= speedDampMult;
			cameraVelocity.z *= speedDampMult;

			bool bInAir = false;
			if (IsInBlockGrid((int)(cameraPosF.x + 0.5f), (int)(cameraPosF.y - 1), (int)(cameraPosF.z + 0.5f)))
			{
				// Touching block				
				bInAir = true;

				if (cameraVelocity.y < 0.0f)
				{
					cameraVelocity.y = 0.0f;
					bJumping = false;
				}

				if (!bJumping && dmwi::isPressed(dmwi::SPACE))
				{
					bJumping = true;
					cameraVelocity.y = JUMP_VELOCITY;
				}
			}

			auto PushOutOfCube = [&cameraPosF](float xOffset, float yOffset, float zOffset)
			{
				if (IsInBlockGrid((int)(cameraPosF.x + xOffset), (int)(cameraPosF.y + yOffset), (int)(cameraPosF.z + zOffset)))
				{
					const int centerX = (int)cameraPosF.x + xOffset;
					const int centerY = (int)cameraPosF.y + yOffset;
					const int centerZ = (int)cameraPosF.z + zOffset;

					const float distX = cameraPosF.x - 0.5f - (float)centerX;
					const float distY = cameraPosF.y - 0.5f - (float)centerY;
					const float distZ = cameraPosF.z - 0.5f - (float)centerZ;

					if (abs(distX) < abs(distZ))
					{
						if (abs(distY) < abs(distX))
						{
							cameraPosF.y -= distY;
						}
						else
						{
							cameraPosF.x -= distX;
						}
					}
					else
					{
						if (abs(distY) < abs(distZ))
						{
							cameraPosF.y -= distY;
						}
						else
						{
							cameraPosF.z -= distZ;
						}
					}
				}
			};

			///PushOutOfCube(-0.5f, -0.5f, 0.5f);
			///PushOutOfCube(-0.5f, 0.5f, 0.5f);
			///PushOutOfCube(-0.5f, -0.5f, -0.5f);
			///PushOutOfCube(-0.5f, 0.5f, -0.5f);
			///PushOutOfCube(-0.5f, -0.5f, 1.5f);
			///PushOutOfCube(-0.5f, 0.5f, 1.5f);

			PushOutOfCube(0.5f, 0.5f, 0.5f);
			PushOutOfCube(0.5f, -0.5f, 0.5f);
			///PushOutOfCube(0.5f, -0.5f, -0.5f);
			///PushOutOfCube(0.5f, 0.5f, -0.5f);
			///PushOutOfCube(0.5f, -0.5f, 1.5f);
			///PushOutOfCube(0.5f, 0.5f, 1.5f);
			///
			///PushOutOfCube(1.5f, -0.5f, 0.5f);
			///PushOutOfCube(1.5f, 0.5f, 0.5f);
			///PushOutOfCube(1.5f, -0.5f, -0.5f);
			///PushOutOfCube(1.5f, 0.5f, -0.5f);
			///PushOutOfCube(1.5f, -0.5f, 1.5f);
			///PushOutOfCube(1.5f, 0.5f, 1.5f);
		}
	}

	ImGui::End();
}

void Game::Tick(float deltaTime)
{
	CreateImGuiWindow(deltaTime);
	GameplayTick(deltaTime);
}

#endif