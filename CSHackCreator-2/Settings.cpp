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
	strcpy_s(CSHackCreator::Settings::szExeFile, MAX_PATH - 1, "MyHack.exe");
	strcpy_s(CSHackCreator::Settings::szDllFile, MAX_PATH - 1, "MyHack.dll");
	strcpy_s(CSHackCreator::Settings::szConfigFile, MAX_PATH - 1, "MyHack.ini");

	// Loader
	strcpy_s(CSHackCreator::Settings::Loader::szTitle, MAX_PATH - 1, "MyHack");
	strcpy_s(CSHackCreator::Settings::Loader::szInject, MAX_PATH - 1, "Inject MyHack");
	strcpy_s(CSHackCreator::Settings::Loader::szWaitingForInjection, MAX_PATH - 1, "Waiting for the game to inject...");
	strcpy_s(CSHackCreator::Settings::Loader::szInjectionError, MAX_PATH - 1, "Injection error!");
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
	if (ImGui::CollapsingHeader("Files", ImGuiTreeNodeFlags_DefaultOpen))
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
	{
		ImGui::PushItemWidth(200);
		ImGui::InputText("Exe file", CSHackCreator::Settings::szExeFile, MAX_PATH - 1);
		ImGui::InputText("Dll file", CSHackCreator::Settings::szDllFile, MAX_PATH - 1);
		ImGui::InputText("Config file", CSHackCreator::Settings::szConfigFile, MAX_PATH - 1);
		ImGui::PopItemWidth();

		ImGui::NewLine();
	}

	if (ImGui::CollapsingHeader("Loader"))
	{
		ImGui::PushItemWidth(400);
		ImGui::InputText("Title##LOADER", CSHackCreator::Settings::Loader::szTitle, MAX_PATH - 1);
		ImGui::InputText("Inject button", CSHackCreator::Settings::Loader::szInject, MAX_PATH - 1);
		ImGui::InputText("Waiting for the game to inject", CSHackCreator::Settings::Loader::szWaitingForInjection, MAX_PATH - 1);
		ImGui::InputText("Injection error", CSHackCreator::Settings::Loader::szInjectionError, MAX_PATH - 1);
		ImGui::PopItemWidth();
		ImGui::NewLine();
	}

	if (ImGui::CollapsingHeader("DLL default settings"))
	{
		ImGui::NewLine();
	}
}