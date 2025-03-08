#pragma once

// STL
#include <string.h>
#include <unordered_map>
#include <map>
#include <vector>
#include <stack>
#include <filesystem>
#include <queue>
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>

// IMGUI
#include <imgui.h>

// VULKAN
#include <vulkan/vulkan.h>

// WINDOWS
#define WIN32_LEAN_AND_MEAN  
#define NOMINMAX
#include <Windows.h>

// MY HEADERS
#include "ImGuiEditorMacros.h"
#include "DomMath/Types.h"
#include "DomLog/DomLog.h"
#include "DomMath/Vec2.h"
#include "DomMath/Vec3.h"
#include "DomMath/Vec4.h"
#include "DomMath/Box2.h"
#include "DomUtils/Pointers.h"
#include "DomUtils/Delegate.h"
#include "Game.h"
#include "AssetManager/AssetManager.h" // Contains utils for ENUMSTRING() so useful to be here