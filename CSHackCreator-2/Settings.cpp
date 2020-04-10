#include "CSHackCreator.h"

char CSHackCreator::Settings::Loader::szTitle[MAX_PATH];
char CSHackCreator::Settings::Loader::szInject[MAX_PATH];
char CSHackCreator::Settings::Loader::szWaitingForInjection[MAX_PATH];
char CSHackCreator::Settings::Loader::szInjectionError[MAX_PATH];

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
}

void CSHackCreator::Interface::Settings()
{
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
	if (ImGui::CollapsingHeader(/*Files*/XorStr<0x77, 6, 0x8BFBC525>("\x31\x11\x15\x1F\x08" + 0x8BFBC525).s, ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::PushItemWidth(200);
		ImGui::InputText(/*Exe file*/XorStr<0xEE, 9, 0xD12006A9>("\xAB\x97\x95\xD1\x94\x9A\x98\x90" + 0xD12006A9).s, CSHackCreator::Settings::szExeFile, MAX_PATH - 1);
		ImGui::InputText(/*Dll file*/XorStr<0x8C, 9, 0x93E3494B>("\xC8\xE1\xE2\xAF\xF6\xF8\xFE\xF6" + 0x93E3494B).s, CSHackCreator::Settings::szDllFile, MAX_PATH - 1);
		ImGui::InputText(/*Config file*/XorStr<0x99, 12, 0xAA5A0D30>("\xDA\xF5\xF5\xFA\xF4\xF9\xBF\xC6\xC8\xCE\xC6" + 0xAA5A0D30).s, CSHackCreator::Settings::szConfigFile, MAX_PATH - 1);
		ImGui::PopItemWidth();

		ImGui::NewLine();
	}

	if (ImGui::CollapsingHeader(/*Loader*/XorStr<0x8D, 7, 0x2E87244C>("\xC1\xE1\xEE\xF4\xF4\xE0" + 0x2E87244C).s))
	{
		ImGui::PushItemWidth(400);
		ImGui::InputText(/*Title##LOADER*/XorStr<0xC2, 14, 0xDD99C4C7>("\x96\xAA\xB0\xA9\xA3\xE4\xEB\x85\x85\x8A\x88\x88\x9C" + 0xDD99C4C7).s, CSHackCreator::Settings::Loader::szTitle, MAX_PATH - 1);
		ImGui::InputText(/*Inject button*/XorStr<0xCE, 14, 0xA14D3C44>("\x87\xA1\xBA\xB4\xB1\xA7\xF4\xB7\xA3\xA3\xAC\xB6\xB4" + 0xA14D3C44).s, CSHackCreator::Settings::Loader::szInject, MAX_PATH - 1);
		ImGui::InputText(/*Waiting for the game to inject*/XorStr<0x7B, 31, 0x674A0E2C>("\x2C\x1D\x14\x0A\x16\xEE\xE6\xA2\xE5\xEB\xF7\xA6\xF3\xE0\xEC\xAA\xEC\xED\xE0\xEB\xAF\xE4\xFE\xB2\xFA\xFA\xFF\xF3\xF4\xEC" + 0x674A0E2C).s, CSHackCreator::Settings::Loader::szWaitingForInjection, MAX_PATH - 1);
		ImGui::InputText(/*Injection error*/XorStr<0xB4, 16, 0x69229FDA>("\xFD\xDB\xDC\xD2\xDB\xCD\xD3\xD4\xD2\x9D\xDB\xCD\xB2\xAE\xB0" + 0x69229FDA).s, CSHackCreator::Settings::Loader::szInjectionError, MAX_PATH - 1);
		ImGui::PopItemWidth();

		ImGui::NewLine();
	}

	if (ImGui::CollapsingHeader(/*DLL default settings*/XorStr<0xF6, 21, 0xEA8CA877>("\xB2\xBB\xB4\xD9\x9E\x9E\x9A\x9C\x8B\x93\x74\x21\x71\x66\x70\x71\x6F\x69\x6F\x7A" + 0xEA8CA877).s))
	{
		ImGui::NewLine();
	}
}