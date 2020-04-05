#pragma once

#ifndef IMGUI_DEFINE_MATH_OPERATORS
#   define IMGUI_DEFINE_MATH_OPERATORS
#endif
#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_dx9.h"
#include "ImGui/imgui_impl_win32.h"
#include "ImGui/ImNodes.h"
#include "ImGui/ImNodesEz.h"
#include <d3d9.h>
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
#include <tchar.h>

#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <string>

#include "JSON/json.h"
#include "JSON/json-forwards.h"

#include "interface_designer.h"
#include "Serialization.h"
#include "Settings.h"