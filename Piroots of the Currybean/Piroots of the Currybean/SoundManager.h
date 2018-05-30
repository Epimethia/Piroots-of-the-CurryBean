#pragma once
#include "Resource.h"

class SoundManager {
public:
	SoundManager();
	~SoundManager();
	bool Init();
	void PlayMusic();
	const bool LoadAudio();
private:
	FMOD::System* audioMgr;
	FMOD::Sound* fxThump;
	FMOD::Sound* bgmTheme;
	FMOD::Channel* channel;
};