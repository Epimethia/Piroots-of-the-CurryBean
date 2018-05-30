#include "SoundManager.h"

SoundManager::SoundManager() {}

SoundManager::~SoundManager() {
	delete audioMgr;
	delete fxThump;
	delete bgmTheme;
	delete channel;
	audioMgr = nullptr;
	fxThump = nullptr;
	bgmTheme = nullptr;
	channel = nullptr;
}

bool SoundManager::Init() {
	if (FMOD::System_Create(&audioMgr) != FMOD_OK) {
		std::cout << "Failed to create Audio Manager \n";
		return false;
	}
	if (audioMgr->init(50, FMOD_INIT_NORMAL, 0) != FMOD_OK) {
		std::cout << "Failed to initialize Audio Manager \n";
		return false;
	}
	std::cout << "Audio Manager created successfully \n";
	return true;
}

void SoundManager::PlayMusic() {
	audioMgr->playSound(bgmTheme, 0, false, &channel);
}

const bool SoundManager::LoadAudio() {
	if (audioMgr->createSound(THUMP_SOUND, FMOD_DEFAULT, 0, &fxThump) != FMOD_OK) {
		std::cout << "Failed to create Sound " << THUMP_SOUND << std::endl;
		return false;
	}
	if (audioMgr->createSound(BG_MUSIC_LOOP, FMOD_DEFAULT, 0, &bgmTheme) != FMOD_OK) {
		std::cout << "Failed to create Sound " << BG_MUSIC_LOOP << std::endl;
		return false;
	}
	bgmTheme->setMode(FMOD_LOOP_NORMAL);
	std::cout << "Music Loaded Successfully\n";
	return true;
}
