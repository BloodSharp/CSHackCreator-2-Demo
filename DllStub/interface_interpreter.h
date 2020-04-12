#pragma once

namespace DllStub
{
	namespace Interface
	{
		void Initialize();
		void LoadInterpreter(Json::Value&settings);
		void Interface();
		void Background();
	}
}
