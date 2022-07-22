#pragma once

#include "../COMPILE_FLAGS.h"

#define _CRT_SECURE_NO_WARNINGS
#ifndef IMGUI_DEFINE_MATH_OPERATORS
#   define IMGUI_DEFINE_MATH_OPERATORS
#endif
#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_opengl3.h"
#include "ImGui/imgui_impl_sdl.h"
#include "ImGui/ImNodes.h"
#include "ImGui/ImNodesEz.h"

#include <SDL.h>
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <SDL_opengles2.h>
#else
#include <SDL_opengl.h>
#endif

#if defined(_WIN32)
#include <SDL_syswm.h>
#endif

/*
#include <d3d9.h>
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
#include <tchar.h>
*/
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

#ifdef CSHACKCREATOR_V2_ENABLE_RADIO
#include "ch4_radio_windows.h"
#endif//CSHACKCREATOR_V2_ENABLE_RADIO

#include "xorstr.h"

#define IDR_DLL_STUB 400
#define IDR_EXE_STUB 401