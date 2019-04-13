#include "AudioManager.h"



CAudioManager::CAudioManager()
{
}


CAudioManager::~CAudioManager()
{
}

bool CAudioManager::Init()
{
	FMOD_RESULT result;
	result = FMOD::System_Create(&m_pAudioMgr);
	if (result != FMOD_OK)
	{
		return(false);
	}

	result = m_pAudioMgr->init(50, FMOD_INIT_NORMAL, 0);
	if (result != FMOD_OK)
	{
		return(false);
	}
	return(true);
}

const bool CAudioManager::LoadAudio(std::string _AudioPath, std::string _KeyName, FMOD_MODE _SoundMode)
{
	FMOD_RESULT result;

	FMOD::Sound* newSound;
	result = m_pAudioMgr->createSound(_AudioPath.c_str(), FMOD_DEFAULT, 0, &newSound);
	
	newSound->setMode(_SoundMode);
	m_Sounds[_KeyName]= newSound;

	return (true);
}

void CAudioManager::PlayAudio(std::string _AudioKey)
{
	m_pAudioMgr->playSound(m_Sounds[_AudioKey], 0, false, &m_pChannel);
}

void CAudioManager::StopAllAudio()
{
	m_pChannel->stop();
}

