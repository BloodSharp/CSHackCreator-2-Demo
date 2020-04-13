#pragma once

#define _CRT_SECURE_NO_WARNINGS
#include <Windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#include <cstdio>
#include <fstream>
#include <corecrt_math_defines.h>
#include <cmath>
#include <algorithm>

#include "ch4_lib_utils.h"

#include "JSON/json.h"
#include "JSON/json-forwards.h"

#include "jsonGetVals.h"

#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_opengl2.h"
#include "ImGui/imgui_impl_win32.h"

#include "xorstr.h"

#include "interface_interpreter.h"

enum LibraryCheckBoxes
{
	LIBRARY_CHECKBOX_MASTER_SWITCH,
	LIBRARY_CHECKBOX_NO_SMOKE,
	LIBRARY_CHECKBOX_NO_FLASH,
	LIBRARY_CHECKBOX_NO_SKY,
	LIBRARY_CHECKBOX_ESP_BOX,
	LIBRARY_CHECKBOX_BUNNY_HOP,
	LIBRARY_CHECKBOX_ZOOM,

	LIBRARY_CHECKBOX_ALL
};

enum LibraryComboBoxes
{
	LIBRARY_COMBOBOX_WALLHACK,
	LIBRARY_COMBOBOX_EXTRAWALLS,
	LIBRARY_COMBOBOX_EXTRAMODELS,
	LIBRARY_COMBOBOX_AIMTEAM,

	LIBRARY_COMBOBOX_ALL
};

enum LibrarySliders
{
	LIBRARY_SLIDER_WIREFRAMEWALL,
	LIBRARY_SLIDER_WIREMODELS,
	LIBRARY_SLIDER_CROSSHAIR,
	LIBRARY_SLIDER_NORECOIL,
	LIBRARY_SLIDER_SPEEDHACK,

	LIBRARY_SLIDER_ALL
};

typedef struct
{
	bool CheckBoxes[LIBRARY_CHECKBOX_ALL];
	int ComboBoxes[LIBRARY_COMBOBOX_ALL];
	int Sliders[LIBRARY_SLIDER_ALL];
	int iZoom;
}cvar;
