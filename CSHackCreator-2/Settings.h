#pragma once

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

namespace CSHackCreator
{
	namespace Settings
	{
		namespace Loader
		{
			extern char szTitle[MAX_PATH];
			extern char szInject[MAX_PATH];
			extern char szWaitingForInjection[MAX_PATH];
			extern char szInjectionError[MAX_PATH];
		}

		namespace Library
		{
			extern bool CheckBoxes[LIBRARY_CHECKBOX_ALL];
			extern int ComboBoxes[LIBRARY_COMBOBOX_ALL];
			extern int Sliders[LIBRARY_SLIDER_ALL];
		}

		void New();
		void Open(Json::Value&settings);
		void Save(Json::Value&settings);

		extern char szExeFile[MAX_PATH];
		extern char szDllFile[MAX_PATH];
		extern char szConfigFile[MAX_PATH];
	}

	namespace Interface
	{
		void Settings();
	}
}

void GetVal(Json::Value& config, int* setting);
void GetVal(Json::Value& config, unsigned int* setting);
void GetVal(Json::Value& config, bool* setting);
void GetVal(Json::Value& config, float* setting);
void GetVal(Json::Value& config, double* setting);
void GetVal(Json::Value& config, char* setting, int bufferSize);