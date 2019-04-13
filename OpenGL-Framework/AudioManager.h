#pragma once
#include "Dependencies\FMOD\fmod.hpp"
#include <string.h>
#include <map>


class CAudioManager
{
public:
	CAudioManager();
	~CAudioManager();

	bool Init();

	const bool LoadAudio(std::string _AudioPath, std::string _KeyName, FMOD_MODE _SoundMode);
	void PlayAudio(std::string _AudioKey);
	void StopAllAudio();

private:
	FMOD::System* m_pAudioMgr;
	FMOD::Channel* m_pChannel;

	std::map<std::string, FMOD::Sound*> m_Sounds;
};

