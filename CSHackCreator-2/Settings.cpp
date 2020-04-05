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