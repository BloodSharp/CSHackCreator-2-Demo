#include "CSHackCreator.h"

char CSHackCreator::Settings::Loader::szTitle[MAX_PATH];
char CSHackCreator::Settings::Loader::szInject[MAX_PATH];
char CSHackCreator::Settings::Loader::szWaitingForInjection[MAX_PATH];
char CSHackCreator::Settings::Loader::szInjectionError[MAX_PATH];
char CSHackCreator::Settings::Loader::szDllNotFound[MAX_PATH];

bool CSHackCreator::Settings::Library::CheckBoxes[LIBRARY_CHECKBOX_ALL];
int  CSHackCreator::Settings::Library::ComboBoxes[LIBRARY_COMBOBOX_ALL];
int  CSHackCreator::Settings::Library::Sliders[LIBRARY_SLIDER_ALL];

char CSHackCreator::Settings::szExeFile[MAX_PATH];
char CSHackCreator::Settings::szDllFile[MAX_PATH];
char CSHackCreator::Settings::szConfigFile[MAX_PATH];

void GetVal(Json::Value& config, int* setting)
{
	if (config.isNull())
		return;

	*setting = config.asInt();
}

void GetVal(Json::Value& config, unsigned int* setting)
{
	if (config.isNull())
		return;

	*setting = config.asUInt();
}

void GetVal(Json::Value& config, bool* setting)
{
	if (config.isNull())
		return;

	*setting = config.asBool();
}

void GetVal(Json::Value& config, float* setting)
{
	if (config.isNull())
		return;

	*setting = config.asFloat();
}

void GetVal(Json::Value& config, double* setting)
{
	if (config.isNull())
		return;

	*setting = config.asDouble();
}

void GetVal(Json::Value& config, char* setting, int bufferSize)
{
	if (config.isNull())
		return;

	strncpy(setting, config.asCString(), bufferSize);
}

void CSHackCreator::Settings::New()
{
	// Files
	strncpy(CSHackCreator::Settings::szExeFile, "MyHack.exe", MAX_PATH - 1);
	strncpy(CSHackCreator::Settings::szDllFile, "MyHack.dll", MAX_PATH - 1);
	strncpy(CSHackCreator::Settings::szConfigFile, "MyHack.ini", MAX_PATH - 1);

	// Loader
	strncpy(CSHackCreator::Settings::Loader::szTitle, "MyHack", MAX_PATH - 1);
	strncpy(CSHackCreator::Settings::Loader::szInject, "Inject MyHack", MAX_PATH - 1);
	strncpy(CSHackCreator::Settings::Loader::szWaitingForInjection, "Waiting for the game to inject...", MAX_PATH - 1);
	strncpy(CSHackCreator::Settings::Loader::szInjectionError, "Injection error!", MAX_PATH - 1);
	strncpy(CSHackCreator::Settings::Loader::szDllNotFound, "MyHack.dll not found!", MAX_PATH - 1);

	// Library
	CSHackCreator::Settings::Library::CheckBoxes[LIBRARY_CHECKBOX_MASTER_SWITCH] = false;
	CSHackCreator::Settings::Library::CheckBoxes[LIBRARY_CHECKBOX_NO_SMOKE] = false;
	CSHackCreator::Settings::Library::CheckBoxes[LIBRARY_CHECKBOX_NO_FLASH] = false;
	CSHackCreator::Settings::Library::CheckBoxes[LIBRARY_CHECKBOX_NO_SKY] = false;
	CSHackCreator::Settings::Library::CheckBoxes[LIBRARY_CHECKBOX_ESP_BOX] = false;
	CSHackCreator::Settings::Library::CheckBoxes[LIBRARY_CHECKBOX_BUNNY_HOP] = false;
	CSHackCreator::Settings::Library::CheckBoxes[LIBRARY_CHECKBOX_ZOOM] = false;

	CSHackCreator::Settings::Library::ComboBoxes[LIBRARY_COMBOBOX_WALLHACK] = 0;
	CSHackCreator::Settings::Library::ComboBoxes[LIBRARY_COMBOBOX_EXTRAWALLS] = 0;
	CSHackCreator::Settings::Library::ComboBoxes[LIBRARY_COMBOBOX_EXTRAMODELS] = 0;
	CSHackCreator::Settings::Library::ComboBoxes[LIBRARY_COMBOBOX_AIMTEAM] = 0;

	CSHackCreator::Settings::Library::Sliders[LIBRARY_SLIDER_WIREFRAMEWALL] = 1;
	CSHackCreator::Settings::Library::Sliders[LIBRARY_SLIDER_WIREMODELS] = 0;
	CSHackCreator::Settings::Library::Sliders[LIBRARY_SLIDER_CROSSHAIR] = 0;
	CSHackCreator::Settings::Library::Sliders[LIBRARY_SLIDER_NORECOIL] = 0;
	CSHackCreator::Settings::Library::Sliders[LIBRARY_SLIDER_SPEEDHACK] = 0;
}

void CSHackCreator::Settings::Open(Json::Value&settings)
{
	// Files
	GetVal(settings["Files"]["Executable"], CSHackCreator::Settings::szExeFile, MAX_PATH - 1);
	GetVal(settings["Files"]["Library"], CSHackCreator::Settings::szDllFile, MAX_PATH - 1);
	GetVal(settings["Files"]["Configurations"], CSHackCreator::Settings::szConfigFile, MAX_PATH - 1);

	// Loader
	GetVal(settings["Loader"]["Title"], CSHackCreator::Settings::Loader::szTitle, MAX_PATH - 1);
	GetVal(settings["Loader"]["Inject"], CSHackCreator::Settings::Loader::szInject, MAX_PATH - 1);
	GetVal(settings["Loader"]["WaitingForTheGame"], CSHackCreator::Settings::Loader::szWaitingForInjection, MAX_PATH - 1);
	GetVal(settings["Loader"]["InjectionError"], CSHackCreator::Settings::Loader::szInjectionError, MAX_PATH - 1);
	GetVal(settings["Loader"]["DllNotFound"], CSHackCreator::Settings::Loader::szDllNotFound, MAX_PATH - 1);

	// Library
	GetVal(settings["Library"]["MasterSwitch"], &CSHackCreator::Settings::Library::CheckBoxes[LIBRARY_CHECKBOX_MASTER_SWITCH]);
	GetVal(settings["Library"]["NoSmoke"], &CSHackCreator::Settings::Library::CheckBoxes[LIBRARY_CHECKBOX_NO_SMOKE]);
	GetVal(settings["Library"]["NoFlash"], &CSHackCreator::Settings::Library::CheckBoxes[LIBRARY_CHECKBOX_NO_FLASH]);
	GetVal(settings["Library"]["NoSky"], &CSHackCreator::Settings::Library::CheckBoxes[LIBRARY_CHECKBOX_NO_SKY]);
	GetVal(settings["Library"]["EspBox"], &CSHackCreator::Settings::Library::CheckBoxes[LIBRARY_CHECKBOX_ESP_BOX]);
	GetVal(settings["Library"]["BunnyHop"], &CSHackCreator::Settings::Library::CheckBoxes[LIBRARY_CHECKBOX_BUNNY_HOP]);
	GetVal(settings["Library"]["Zoom"], &CSHackCreator::Settings::Library::CheckBoxes[LIBRARY_CHECKBOX_ZOOM]);

	GetVal(settings["Library"]["WallHack"], &CSHackCreator::Settings::Library::ComboBoxes[LIBRARY_COMBOBOX_WALLHACK]);
	GetVal(settings["Library"]["ExtraWalls"], &CSHackCreator::Settings::Library::ComboBoxes[LIBRARY_COMBOBOX_EXTRAWALLS]);
	GetVal(settings["Library"]["ExtraModels"], &CSHackCreator::Settings::Library::ComboBoxes[LIBRARY_COMBOBOX_EXTRAMODELS]);
	GetVal(settings["Library"]["AimTeam"], &CSHackCreator::Settings::Library::ComboBoxes[LIBRARY_COMBOBOX_AIMTEAM]);

	GetVal(settings["Library"]["WireFrameWall"], &CSHackCreator::Settings::Library::Sliders[LIBRARY_SLIDER_WIREFRAMEWALL]);
	GetVal(settings["Library"]["WireModels"], &CSHackCreator::Settings::Library::Sliders[LIBRARY_SLIDER_WIREMODELS]);
	GetVal(settings["Library"]["CrossHair"], &CSHackCreator::Settings::Library::Sliders[LIBRARY_SLIDER_CROSSHAIR]);
	GetVal(settings["Library"]["Recoil"], &CSHackCreator::Settings::Library::Sliders[LIBRARY_SLIDER_NORECOIL]);
	GetVal(settings["Library"]["Speed"], &CSHackCreator::Settings::Library::Sliders[LIBRARY_SLIDER_SPEEDHACK]);
}

void CSHackCreator::Settings::Save(Json::Value&settings)
{
	// Files
	settings["Files"]["Executable"] = CSHackCreator::Settings::szExeFile;
	settings["Files"]["Library"] = CSHackCreator::Settings::szDllFile;
	settings["Files"]["Configurations"] = CSHackCreator::Settings::szConfigFile;

	// Loader
	settings["Loader"]["Title"] = CSHackCreator::Settings::Loader::szTitle;
	settings["Loader"]["Inject"] = CSHackCreator::Settings::Loader::szInject;
	settings["Loader"]["WaitingForTheGame"] = CSHackCreator::Settings::Loader::szWaitingForInjection;
	settings["Loader"]["InjectionError"] = CSHackCreator::Settings::Loader::szInjectionError;
	settings["Loader"]["DllNotFound"] = CSHackCreator::Settings::Loader::szDllNotFound;

	// Library
	settings["Library"]["MasterSwitch"] = CSHackCreator::Settings::Library::CheckBoxes[LIBRARY_CHECKBOX_MASTER_SWITCH];
	settings["Library"]["NoSmoke"] = CSHackCreator::Settings::Library::CheckBoxes[LIBRARY_CHECKBOX_NO_SMOKE];
	settings["Library"]["NoFlash"] = CSHackCreator::Settings::Library::CheckBoxes[LIBRARY_CHECKBOX_NO_FLASH];
	settings["Library"]["NoSky"] = CSHackCreator::Settings::Library::CheckBoxes[LIBRARY_CHECKBOX_NO_SKY];
	settings["Library"]["EspBox"] = CSHackCreator::Settings::Library::CheckBoxes[LIBRARY_CHECKBOX_ESP_BOX];
	settings["Library"]["BunnyHop"] = CSHackCreator::Settings::Library::CheckBoxes[LIBRARY_CHECKBOX_BUNNY_HOP];
	settings["Library"]["Zoom"] = CSHackCreator::Settings::Library::CheckBoxes[LIBRARY_CHECKBOX_ZOOM];

	settings["Library"]["WallHack"] = CSHackCreator::Settings::Library::ComboBoxes[LIBRARY_COMBOBOX_WALLHACK];
	settings["Library"]["ExtraWalls"] = CSHackCreator::Settings::Library::ComboBoxes[LIBRARY_COMBOBOX_EXTRAWALLS];
	settings["Library"]["ExtraModels"] = CSHackCreator::Settings::Library::ComboBoxes[LIBRARY_COMBOBOX_EXTRAMODELS];
	settings["Library"]["AimTeam"] = CSHackCreator::Settings::Library::ComboBoxes[LIBRARY_COMBOBOX_AIMTEAM];

	settings["Library"]["WireFrameWall"] = CSHackCreator::Settings::Library::Sliders[LIBRARY_SLIDER_WIREFRAMEWALL];
	settings["Library"]["WireModels"] = CSHackCreator::Settings::Library::Sliders[LIBRARY_SLIDER_WIREMODELS];
	settings["Library"]["CrossHair"] = CSHackCreator::Settings::Library::Sliders[LIBRARY_SLIDER_CROSSHAIR];
	settings["Library"]["Recoil"] = CSHackCreator::Settings::Library::Sliders[LIBRARY_SLIDER_NORECOIL];
	settings["Library"]["Speed"] = CSHackCreator::Settings::Library::Sliders[LIBRARY_SLIDER_SPEEDHACK];
}

void CSHackCreator::Interface::Settings()
{
	#ifdef CSHACKCREATOR_V2_ENABLE_RADIO
	if (ImGui::CollapsingHeader(/*Radio (Enjoy music while building your hack)*/XorStr<0x42, 45, 0x0954A532>("\x10\x22\x20\x2C\x29\x67\x60\x0C\x24\x21\x23\x34\x6E\x22\x25\x22\x3B\x30\x74\x22\x3E\x3E\x34\x3C\x7A\x39\x29\x34\x32\x3B\x09\x0F\x05\x43\x1D\x0A\x13\x15\x48\x01\x0B\x08\x07\x44" + 0x0954A532).s, ImGuiTreeNodeFlags_DefaultOpen))
	{
		const char* listbox_radio[] = {
			"Radio Off",
			"PulseEDM Dance Music (USA)",
			"Hard Style (UK)",
			"Big Fm (Deutsch)",
			"Big Fm Rap (Deutsch)",
			"Radio Record (Rusia)",
			"Record Dubstep (Rusia)",
			"Record Hardstyle (Rusia)",
			"Record Dancecore (Rusia)",
			"Anison FM (Rusia)",
			"Vorterix FM 92.1 (Argentina)",
			"Disney FM 94.3 (Argentina)",
			"La 100 FM 99.9 (Argentina)",
			"Latina FM 101.1 (Argentina)",
			"Los 40 FM 105.5 (Argentina)"
		};
		ImGui::PushItemWidth(300);
		if (ImGui::Combo(/*Channel*/XorStr<0x04, 8, 0xA6E46ADE>("\x47\x6D\x67\x69\x66\x6C\x66" + 0xA6E46ADE).s, &CH4::Radio::Channel::Current, listbox_radio, IM_ARRAYSIZE(listbox_radio), 7))
			CH4::Radio::Channel::Change = true;
		ImGui::SliderFloat(/*Volume*/XorStr<0x16, 7, 0x70B59499>("\x40\x78\x74\x6C\x77\x7E" + 0x70B59499).s, &CH4::Radio::Channel::Volume, 0.f, 100.f, /*%.0f*/XorStr<0xF3, 5, 0x8C91422A>("\xD6\xDA\xC5\x90" + 0x8C91422A).s);
		ImGui::PopItemWidth();

		ImGui::NewLine();
	}
	#endif//CSHACKCREATOR_V2_ENABLE_RADIO
	if (ImGui::CollapsingHeader("Files", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::PushItemWidth(200);
		ImGui::InputText("Exe file", CSHackCreator::Settings::szExeFile, MAX_PATH - 1);
		ImGui::InputText("Dll file", CSHackCreator::Settings::szDllFile, MAX_PATH - 1);
		ImGui::InputText("Config file", CSHackCreator::Settings::szConfigFile, MAX_PATH - 1);
		ImGui::PopItemWidth();

		ImGui::NewLine();
	}

	if (ImGui::CollapsingHeader("Loader", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::PushItemWidth(400);
		ImGui::InputText("Title##LOADER", CSHackCreator::Settings::Loader::szTitle, MAX_PATH - 1);
		ImGui::InputText("Inject button", CSHackCreator::Settings::Loader::szInject, MAX_PATH - 1);
		ImGui::InputText("Waiting for the game to inject", CSHackCreator::Settings::Loader::szWaitingForInjection, MAX_PATH - 1);
		ImGui::InputText("Injection error", CSHackCreator::Settings::Loader::szInjectionError, MAX_PATH - 1);
		ImGui::InputText("DLL not found", CSHackCreator::Settings::Loader::szDllNotFound, MAX_PATH - 1);
		ImGui::PopItemWidth();

		ImGui::NewLine();
	}

	if (ImGui::CollapsingHeader("DLL default settings", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::Checkbox("Master Switch", &CSHackCreator::Settings::Library::CheckBoxes[LIBRARY_CHECKBOX_MASTER_SWITCH]);
		ImGui::Separator();
		ImGui::BeginGroup();
		ImGui::Checkbox("No Smoke", &CSHackCreator::Settings::Library::CheckBoxes[LIBRARY_CHECKBOX_NO_SMOKE]);
		ImGui::Checkbox("No Flash", &CSHackCreator::Settings::Library::CheckBoxes[LIBRARY_CHECKBOX_NO_FLASH]);
		ImGui::Checkbox("No Sky", &CSHackCreator::Settings::Library::CheckBoxes[LIBRARY_CHECKBOX_NO_SKY]);
		ImGui::EndGroup();

		ImGui::SameLine();
		ImGui::BeginGroup();
		ImGui::Checkbox("ESP Box", &CSHackCreator::Settings::Library::CheckBoxes[LIBRARY_CHECKBOX_ESP_BOX]);
		ImGui::Checkbox("Bunny Hop", &CSHackCreator::Settings::Library::CheckBoxes[LIBRARY_CHECKBOX_BUNNY_HOP]);
		ImGui::Checkbox("Zoom", &CSHackCreator::Settings::Library::CheckBoxes[LIBRARY_CHECKBOX_ZOOM]);
		ImGui::EndGroup();

		ImGui::PushItemWidth(105);
		ImGui::Combo("Wall Hack", &CSHackCreator::Settings::Library::ComboBoxes[LIBRARY_COMBOBOX_WALLHACK], "Disabled\0Models\0Transparent\0Wireframe\0\0");
		ImGui::Combo("Extra Walls", &CSHackCreator::Settings::Library::ComboBoxes[LIBRARY_COMBOBOX_EXTRAWALLS], "Disabled\0Full Bright\0White Walls\0Night Mode\0\0");
		ImGui::Combo("Extra Models", &CSHackCreator::Settings::Library::ComboBoxes[LIBRARY_COMBOBOX_EXTRAMODELS], "Disabled\0Lambert\0Dark\0Colored\0\0");
		ImGui::Combo("Aim Team", &CSHackCreator::Settings::Library::ComboBoxes[LIBRARY_COMBOBOX_AIMTEAM], "Disabled\0CTs\0TTs\0\0");

		ImGui::SliderInt("WireFrame Wall", &CSHackCreator::Settings::Library::Sliders[LIBRARY_SLIDER_WIREFRAMEWALL], 1, 5);
		ImGui::SliderInt("Wire Models", &CSHackCreator::Settings::Library::Sliders[LIBRARY_SLIDER_WIREMODELS], 0, 5);
		ImGui::SliderInt("CrossHair", &CSHackCreator::Settings::Library::Sliders[LIBRARY_SLIDER_CROSSHAIR], 0, 5);
		ImGui::SliderInt("No Recoil", &CSHackCreator::Settings::Library::Sliders[LIBRARY_SLIDER_NORECOIL], 0, 5);
		ImGui::SliderInt("Speed Hack", &CSHackCreator::Settings::Library::Sliders[LIBRARY_SLIDER_SPEEDHACK], -9, 9);
		ImGui::PopItemWidth();

		ImGui::NewLine();
	}
}