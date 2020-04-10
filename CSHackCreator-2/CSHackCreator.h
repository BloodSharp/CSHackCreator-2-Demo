#pragma once

#define _CRT_SECURE_NO_WARNINGS
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

#include "ch4_radio_windows.h"

#define IDR_DLL_STUB 400
#define IDR_EXE_STUB 401