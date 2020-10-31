#include "SoundManager.h"

irrklang::ISoundEngine* SoundManager::engine;
irrklang::ISound* SoundManager::backgroundMusic;

void SoundManager::init() {
	if (!engine) {
		engine = irrklang::createIrrKlangDevice();
	}
}

void SoundManager::playWin() {
	if (engine)engine->play2D(WIN_SOUND.c_str(), false);
}

void SoundManager::playMenu() {
	if (engine) engine->play2D(MENU_SOUND.c_str(), true);
}

void SoundManager::stopMenu() {
	if (engine)engine->removeSoundSource(MENU_SOUND.c_str());
}

void SoundManager::playMenuMove() {
	if (engine) {
		engine->play2D(MENU_MOVE.c_str(), false);
	}
}

void SoundManager::playMenuSelect() {
	if (engine)engine->play2D(MENU_SELECT.c_str(), false);
}