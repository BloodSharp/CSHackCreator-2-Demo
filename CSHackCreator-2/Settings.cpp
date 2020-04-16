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

	strcpy_s(setting, bufferSize, config.asCString());
}

void CSHackCreator::Settings::New()
{
	// Files
	strcpy_s(CSHackCreator::Settings::szExeFile, MAX_PATH - 1, /*MyHack.exe*/XorStr<0x19, 11, 0x4B1C0520>("\x54\x63\x53\x7D\x7E\x75\x31\x45\x59\x47" + 0x4B1C0520).s);
	strcpy_s(CSHackCreator::Settings::szDllFile, MAX_PATH - 1, /*MyHack.dll*/XorStr<0x6A, 11, 0x50B24B11>("\x27\x12\x24\x0C\x0D\x04\x5E\x15\x1E\x1F" + 0x50B24B11).s);
	strcpy_s(CSHackCreator::Settings::szConfigFile, MAX_PATH - 1, /*MyHack.ini*/XorStr<0xDA, 11, 0x6D2701E0>("\x97\xA2\x94\xBC\xBD\xB4\xCE\x88\x8C\x8A" + 0x6D2701E0).s);

	// Loader
	strcpy_s(CSHackCreator::Settings::Loader::szTitle, MAX_PATH - 1, /*MyHack*/XorStr<0x41, 7, 0x2704D9EB>("\x0C\x3B\x0B\x25\x26\x2D" + 0x2704D9EB).s);
	strcpy_s(CSHackCreator::Settings::Loader::szInject, MAX_PATH - 1, /*Inject MyHack*/XorStr<0x13, 14, 0x82A9F5E6>("\x5A\x7A\x7F\x73\x74\x6C\x39\x57\x62\x54\x7C\x7D\x74" + 0x82A9F5E6).s);
	strcpy_s(CSHackCreator::Settings::Loader::szWaitingForInjection, MAX_PATH - 1, /*Waiting for the game to inject...*/XorStr<0xCC, 34, 0x1505011D>("\x9B\xAC\xA7\xBB\xB9\xBF\xB5\xF3\xB2\xBA\xA4\xF7\xAC\xB1\xBF\xFB\xBB\xBC\xB3\xBA\xC0\x95\x8D\xC3\x8D\x8B\x8C\x82\x8B\x9D\xC4\xC5\xC2" + 0x1505011D).s);
	strcpy_s(CSHackCreator::Settings::Loader::szInjectionError, MAX_PATH - 1, /*Injection error!*/XorStr<0xD2, 17, 0x459D0313>("\x9B\xBD\xBE\xB0\xB5\xA3\xB1\xB6\xB4\xFB\xB9\xAF\xAC\xB0\x92\xC0" + 0x459D0313).s);
	strcpy_s(CSHackCreator::Settings::Loader::szDllNotFound, MAX_PATH - 1,/*MyHack.dll not found!*/XorStr<0x7E, 22, 0xAD2C194E>("\x33\x06\xC8\xE0\xE1\xE8\xAA\xE1\xEA\xEB\xA8\xE7\xE5\xFF\xAC\xEB\xE1\xFA\xFE\xF5\xB3" + 0xAD2C194E).s);

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
	if (ImGui::CollapsingHeader(/*Files*/XorStr<0x77, 6, 0x8BFBC525>("\x31\x11\x15\x1F\x08" + 0x8BFBC525).s, ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::PushItemWidth(200);
		ImGui::InputText(/*Exe file*/XorStr<0xEE, 9, 0xD12006A9>("\xAB\x97\x95\xD1\x94\x9A\x98\x90" + 0xD12006A9).s, CSHackCreator::Settings::szExeFile, MAX_PATH - 1);
		ImGui::InputText(/*Dll file*/XorStr<0x8C, 9, 0x93E3494B>("\xC8\xE1\xE2\xAF\xF6\xF8\xFE\xF6" + 0x93E3494B).s, CSHackCreator::Settings::szDllFile, MAX_PATH - 1);
		ImGui::InputText(/*Config file*/XorStr<0x99, 12, 0xAA5A0D30>("\xDA\xF5\xF5\xFA\xF4\xF9\xBF\xC6\xC8\xCE\xC6" + 0xAA5A0D30).s, CSHackCreator::Settings::szConfigFile, MAX_PATH - 1);
		ImGui::PopItemWidth();

		ImGui::NewLine();
	}

	if (ImGui::CollapsingHeader(/*Loader*/XorStr<0x8D, 7, 0x2E87244C>("\xC1\xE1\xEE\xF4\xF4\xE0" + 0x2E87244C).s, ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::PushItemWidth(400);
		ImGui::InputText(/*Title##LOADER*/XorStr<0xC2, 14, 0xDD99C4C7>("\x96\xAA\xB0\xA9\xA3\xE4\xEB\x85\x85\x8A\x88\x88\x9C" + 0xDD99C4C7).s, CSHackCreator::Settings::Loader::szTitle, MAX_PATH - 1);
		ImGui::InputText(/*Inject button*/XorStr<0xCE, 14, 0xA14D3C44>("\x87\xA1\xBA\xB4\xB1\xA7\xF4\xB7\xA3\xA3\xAC\xB6\xB4" + 0xA14D3C44).s, CSHackCreator::Settings::Loader::szInject, MAX_PATH - 1);
		ImGui::InputText(/*Waiting for the game to inject*/XorStr<0x7B, 31, 0x674A0E2C>("\x2C\x1D\x14\x0A\x16\xEE\xE6\xA2\xE5\xEB\xF7\xA6\xF3\xE0\xEC\xAA\xEC\xED\xE0\xEB\xAF\xE4\xFE\xB2\xFA\xFA\xFF\xF3\xF4\xEC" + 0x674A0E2C).s, CSHackCreator::Settings::Loader::szWaitingForInjection, MAX_PATH - 1);
		ImGui::InputText(/*Injection error*/XorStr<0xB4, 16, 0x69229FDA>("\xFD\xDB\xDC\xD2\xDB\xCD\xD3\xD4\xD2\x9D\xDB\xCD\xB2\xAE\xB0" + 0x69229FDA).s, CSHackCreator::Settings::Loader::szInjectionError, MAX_PATH - 1);
		ImGui::InputText(/*DLL not found*/XorStr<0xB2, 14, 0xBE1C5C54>("\xF6\xFF\xF8\x95\xD8\xD8\xCC\x99\xDC\xD4\xC9\xD3\xDA" + 0xBE1C5C54).s, CSHackCreator::Settings::Loader::szDllNotFound, MAX_PATH - 1);
		ImGui::PopItemWidth();

		ImGui::NewLine();
	}

	if (ImGui::CollapsingHeader(/*DLL default settings*/XorStr<0xF6, 21, 0xEA8CA877>("\xB2\xBB\xB4\xD9\x9E\x9E\x9A\x9C\x8B\x93\x74\x21\x71\x66\x70\x71\x6F\x69\x6F\x7A" + 0xEA8CA877).s, ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::Checkbox(/*Master Switch*/XorStr<0xF5, 14, 0x03D0402E>("\xB8\x97\x84\x8C\x9C\x88\xDB\xAF\x8A\x97\x8B\x63\x69" + 0x03D0402E).s, &CSHackCreator::Settings::Library::CheckBoxes[LIBRARY_CHECKBOX_MASTER_SWITCH]);
		ImGui::Separator();
		ImGui::BeginGroup();
		ImGui::Checkbox(/*No Smoke*/XorStr<0xC9, 9, 0x02D7C8F8>("\x87\xA5\xEB\x9F\xA0\xA1\xA4\xB5" + 0x02D7C8F8).s, &CSHackCreator::Settings::Library::CheckBoxes[LIBRARY_CHECKBOX_NO_SMOKE]);
		ImGui::Checkbox(/*No Flash*/XorStr<0x49, 9, 0x4E30F6DC>("\x07\x25\x6B\x0A\x21\x2F\x3C\x38" + 0x4E30F6DC).s, &CSHackCreator::Settings::Library::CheckBoxes[LIBRARY_CHECKBOX_NO_FLASH]);
		ImGui::Checkbox(/*No Sky*/XorStr<0xFF, 7, 0xD6A5E21C>("\xB1\x6F\x21\x51\x68\x7D" + 0xD6A5E21C).s, &CSHackCreator::Settings::Library::CheckBoxes[LIBRARY_CHECKBOX_NO_SKY]);
		ImGui::EndGroup();

		ImGui::SameLine();
		ImGui::BeginGroup();
		ImGui::Checkbox(/*ESP Box*/XorStr<0xB1, 8, 0x4560A231>("\xF4\xE1\xE3\x94\xF7\xD9\xCF" + 0x4560A231).s, &CSHackCreator::Settings::Library::CheckBoxes[LIBRARY_CHECKBOX_ESP_BOX]);
		ImGui::Checkbox(/*Bunny Hop*/XorStr<0x6C, 10, 0x5BB14CFD>("\x2E\x18\x00\x01\x09\x51\x3A\x1C\x04" + 0x5BB14CFD).s, &CSHackCreator::Settings::Library::CheckBoxes[LIBRARY_CHECKBOX_BUNNY_HOP]);
		ImGui::Checkbox(/*Zoom*/XorStr<0xE6, 5, 0xF5C3C47A>("\xBC\x88\x87\x84" + 0xF5C3C47A).s, &CSHackCreator::Settings::Library::CheckBoxes[LIBRARY_CHECKBOX_ZOOM]);
		ImGui::EndGroup();

		ImGui::PushItemWidth(105);
		ImGui::Combo(/*Wall Hack*/XorStr<0xB9, 10, 0x9E958137>("\xEE\xDB\xD7\xD0\x9D\xF6\xDE\xA3\xAA" + 0x9E958137).s, &CSHackCreator::Settings::Library::ComboBoxes[LIBRARY_COMBOBOX_WALLHACK], /*Disabled\0Models\0Transparent\0Wireframe\0\0*/XorStr<0xD9, 40, 0x24A3BBFD>("\x9D\xB3\xA8\xBD\xBF\xB2\xBA\x84\xE1\xAF\x8C\x80\x80\x8A\x94\xE8\xBD\x98\x8A\x82\x9E\x9E\x8E\x82\x94\x9C\x87\xF4\xA2\x9F\x85\x9D\x9F\x88\x9A\x91\x98\xFE\xFF" + 0x24A3BBFD).s);
		ImGui::Combo(/*Extra Walls*/XorStr<0x2C, 12, 0x668204E6>("\x69\x55\x5A\x5D\x51\x11\x65\x52\x58\x59\x45" + 0x668204E6).s, &CSHackCreator::Settings::Library::ComboBoxes[LIBRARY_COMBOBOX_EXTRAWALLS], /*Disabled\0Full Bright\0White Walls\0Night Mode\0\0*/XorStr<0x49, 46, 0x06ACEC05>("\x0D\x23\x38\x2D\x2F\x22\x2A\x34\x51\x14\x26\x38\x39\x76\x15\x2A\x30\x3D\x33\x28\x5D\x09\x37\x09\x15\x07\x43\x33\x04\x0A\x0B\x1B\x69\x24\x02\x0B\x05\x1A\x4F\x3D\x1E\x16\x16\x74\x75" + 0x06ACEC05).s);
		ImGui::Combo(/*Extra Models*/XorStr<0x27, 13, 0xE10FB907>("\x62\x50\x5D\x58\x4A\x0C\x60\x41\x4B\x55\x5D\x41" + 0xE10FB907).s, &CSHackCreator::Settings::Library::ComboBoxes[LIBRARY_COMBOBOX_EXTRAMODELS], /*Disabled\0Lambert\0Dark\0Colored\0\0*/XorStr<0x30, 32, 0xB964B414>("\x74\x58\x41\x52\x56\x59\x53\x53\x38\x75\x5B\x56\x5E\x58\x4C\x4B\x40\x05\x23\x31\x2F\x45\x05\x28\x24\x26\x38\x2E\x28\x4D\x4E" + 0xB964B414).s);
		ImGui::Combo(/*Aim Team*/XorStr<0x78, 9, 0x8F0DCED1>("\x39\x10\x17\x5B\x28\x18\x1F\x12" + 0x8F0DCED1).s, &CSHackCreator::Settings::Library::ComboBoxes[LIBRARY_COMBOBOX_AIMTEAM], /*Disabled\0CTs\0TTs\0\0*/XorStr<0xAF, 19, 0xAE69E67D>("\xEB\xD9\xC2\xD3\xD1\xD8\xD0\xD2\xB7\xFB\xED\xC9\xBB\xE8\xE9\xCD\xBF\xC0" + 0xAE69E67D).s);

		ImGui::SliderInt(/*WireFrame Wall*/XorStr<0x24, 15, 0xE34A7739>("\x73\x4C\x54\x42\x6E\x5B\x4B\x46\x49\x0D\x79\x4E\x5C\x5D" + 0xE34A7739).s, &CSHackCreator::Settings::Library::Sliders[LIBRARY_SLIDER_WIREFRAMEWALL], 1, 5);
		ImGui::SliderInt(/*Wire Models*/XorStr<0x4A, 12, 0xEE40A126>("\x1D\x22\x3E\x28\x6E\x02\x3F\x35\x37\x3F\x27" + 0xEE40A126).s, &CSHackCreator::Settings::Library::Sliders[LIBRARY_SLIDER_WIREMODELS], 0, 5);
		ImGui::SliderInt(/*CrossHair*/XorStr<0x29, 10, 0x48D66C0F>("\x6A\x58\x44\x5F\x5E\x66\x4E\x59\x43" + 0x48D66C0F).s, &CSHackCreator::Settings::Library::Sliders[LIBRARY_SLIDER_CROSSHAIR], 0, 5);
		ImGui::SliderInt(/*No Recoil*/XorStr<0xA6, 10, 0x9544B0A2>("\xE8\xC8\x88\xFB\xCF\xC8\xC3\xC4\xC2" + 0x9544B0A2).s, &CSHackCreator::Settings::Library::Sliders[LIBRARY_SLIDER_NORECOIL], 0, 5);
		ImGui::SliderInt(/*Speed Hack*/XorStr<0xE7, 11, 0xEE275B68>("\xB4\x98\x8C\x8F\x8F\xCC\xA5\x8F\x8C\x9B" + 0xEE275B68).s, &CSHackCreator::Settings::Library::Sliders[LIBRARY_SLIDER_SPEEDHACK], -9, 9);
		ImGui::PopItemWidth();

		ImGui::NewLine();
	}
}