#include "SoundManager.h"

irrklang::ISoundEngine* SoundManager::engine;
irrklang::ISound* SoundManager::backgroundMusic;

void SoundManager::init() {
	if (!engine) {
		engine = irrklang::createIrrKlangDevice();
	}
}

void SoundManager::playMenu() {
	if (engine) {
		engine->play2D(MENU_SOUND.c_str(), true);
	}
}

void SoundManager::stopSound(string song) {
	if (engine)engine->removeSoundSource(song.c_str());
}

void SoundManager::playMenuMove() {
	if (engine) {
		engine->play2D(MENU_MOVE.c_str(), false);
	}
}

void SoundManager::playMenuSelect() {
	if (engine)engine->play2D(MENU_SELECT.c_str(), false);
}

void SoundManager::playBrick() {
	if (engine)engine->play2D(BRICK_SOUND.c_str(), false);
}

void SoundManager::playCalculator() {
	if (engine)engine->play2D(CALCULATOR_SOUND.c_str(), true);
}

void SoundManager::playWall() {
	if (engine)engine->play2D(WALL_SOUND.c_str(), false);
}

void SoundManager::playPlayer() {
	if (engine)engine->play2D(PLAYER_SOUND.c_str(), false);
}

void SoundManager::playMoney() {
	if (engine)engine->play2D(MONEY_SOUND.c_str(), false);
}

void SoundManager::playPowerUp() {
	if (engine)engine->play2D(POWERUP_SOUND.c_str(), false);
}

void SoundManager::playGameover() {
	if (engine)engine->play2D(GAMEOVER_SOUND.c_str(), false);
}

void SoundManager::playWin() {
	if (engine)engine->play2D(WIN_SOUND.c_str(), false);
}

void SoundManager::playKey() {
	if (engine)engine->play2D(KEY_SOUND.c_str(), false);
}

void SoundManager::playAlarm() {
	if (engine)engine->play2D(ALARM_SOUND.c_str(), true);
}

void SoundManager::playDeath() {
	if (engine)engine->play2D(DEATH_SOUND.c_str(), false);
}

void SoundManager::playGodmode() {
	if (engine)engine->play2D(GODMODE_SOUND.c_str(), false);
}

void SoundManager::playNormalmode() {
	if (engine)engine->play2D(NORMALMODE_SOUND.c_str(), false);
}

bool SoundManager::isCurrentlyPlaying(string song) {
	if (engine) return engine->isCurrentlyPlaying(song.c_str());
}

void SoundManager::playBackground() {
	if (engine) {
		backgroundMusic = engine->play2D(BACKGROUND_SOUND.c_str(), true, false, true);
		backgroundMusic->setVolume(0.06);
	}
}

void SoundManager::stopBackground() {
	if (backgroundMusic) {
		backgroundMusic->stop();
		backgroundMusic->drop();
	}
}
