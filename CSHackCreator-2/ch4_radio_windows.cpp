#include "CSHackCreator.h"
#include "ch4_radio_windows.h"
#include "Bass/dll.h"
#include "Bass/bass.h"

#include <string>

bool CH4::Radio::Channel::Change = false;
int CH4::Radio::Channel::Current = 0;
float CH4::Radio::Channel::Volume = 100;

DWORD dwStream = 0;

void CH4::Radio::Initialize()
{
	BASS::bass_lib_handle = BASS::bass_lib.LoadFromMemory(bass_dll_image, sizeof(bass_dll_image));
	static bool bOnce = false;
	if (!bOnce)
	{
		if (BASS_Init(-1, 44100, 0, nullptr, nullptr))
		{
			BASS_SetConfig(BASS_CONFIG_NET_PLAYLIST, 1);
			BASS_SetConfig(BASS_CONFIG_NET_PREBUF, 0);
			bOnce = true;
		}
	}
}

std::string zstations[] = {
			"Not Used",
			"http://pulseedm.cdnstream1.com:8124/1373_128",
			"http://uk5.internet-radio.com:8270/",
			"http://streams.bigfm.de/bigfm-deutschland-128-mp3",
			"https://streams.bigfm.de/bigfm-deutschrap-128-mp3",
			"http://air2.radiorecord.ru:805/rr_320",
			"http://air.radiorecord.ru:805/dub_320",
			"http://air.radiorecord.ru:805/teo_320",
			"http://air.radiorecord.ru:805/dc_320",
			"http://pool.anison.fm:9000/AniSonFM(320)?nocache=0.752104723294601",
			"http://147.135.11.82:9300/",
			"https://us-b4-p-e-by2-audio.cdn.mdstrm.com/live-audio/5b7dcf9b7f880d076b1ed74d/5db1aedcf0829139c07b1285/icecast.audio",
			"http://retransmisorasenelpais.cienradios.com.ar:8000/la100.aac",
			"http://streaming.latina101.com.ar:8080/RadioLatina",
			"https://playerservices.streamtheworld.com/api/livestream-redirect/LOS40_ARGENTINA_SC"//"https://19193.live.streamtheworld.com/LOS40_ARGENTINA_SC"
			/*
			"https://mega.stweb.tv/mega983/live/chunks.m3u8?nimblesessionid=30484347"
			//"https://server1.stweb.tv/mega983/live/chunks.m3u8"//"https://mega.stweb.tv/mega983/live/playlist.m3u8"
			*/
};

void CH4::Radio::Play()
{
	if (CH4::Radio::Channel::Change)
	{
		BASS_ChannelStop(dwStream);
		dwStream = NULL;
		dwStream = BASS_StreamCreateURL(zstations[CH4::Radio::Channel::Current].c_str(), 0, 0, NULL, 0);
		CH4::Radio::Channel::Change = false;
	}
	if (CH4::Radio::Channel::Current)
	{
		static bool radioInit = false;
		if (!radioInit)
		{
			BASS_Init(-1, 44100, BASS_DEVICE_3D, 0, NULL);
			dwStream = BASS_StreamCreateURL(zstations[CH4::Radio::Channel::Current].c_str(), 0, 0, NULL, 0);
			radioInit = true;
		}
		BASS_ChannelSetAttribute(dwStream, BASS_ATTRIB_VOL, CH4::Radio::Channel::Volume / 100.0f);
		BASS_ChannelPlay(dwStream, false);
	}
}