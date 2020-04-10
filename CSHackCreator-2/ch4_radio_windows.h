#ifndef CH4_RADIO_WINDOWS_H
#define CH4_RADIO_WINDOWS_H

namespace CH4
{
	namespace Radio
	{
		void Initialize();
		void Play();
		namespace Channel
		{
			extern bool Change;
			extern int Current;
			extern float Volume;
		}
	}
}

#endif//CH4_RADIO_WINDOWS_H