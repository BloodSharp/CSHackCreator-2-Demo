#pragma once

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
void GetVal(Json::Value& config, bool* setting);
void GetVal(Json::Value& config, float* setting);
void GetVal(Json::Value& config, char* setting, int bufferSize);