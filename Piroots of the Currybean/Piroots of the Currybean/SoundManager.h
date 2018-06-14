#pragma once
#include "Resource.h"

class SoundManager {
public:
	SoundManager();
	~SoundManager();
	bool Init();
	void PlayBGM();
	void PlayShootFX();
	const bool LoadAudio();
private:
	FMOD::System* audioMgr;
	FMOD::Sound* bgmTheme;
	FMOD::Sound* ShootFX;
	FMOD::Channel* channel;
};