#ifndef _SOUND_MANAGER_INCLUDE
#define _SOUND_MANAGER_INCLUDE
#include "irrKlang.h"
#include <string>
using namespace std;

class SoundManager
{
public:
	void init();
	void playWin();
	void playMenu();
	void stopSound(string song);
	void playMenuMove();
	void playMenuSelect();
	void playBrick();
	void playCalculator();
	void playMoney();
	void playPlayer();
	void playWall();
	void playGameover();
	void playPowerUp();
	void playKey();
	void playAlarm();
	void playDeath();
	bool isCurrentlyPlaying(string song);

	void playBackground();
	void stopBackground();

private:
	string WIN_SOUND = "sound/win.ogg";
	string MENU_SOUND = "sound/menu.ogg";
	string MENU_MOVE = "sound/menu_move.ogg";
	string MENU_SELECT = "sound/menu_select.ogg";
	string BRICK_SOUND = "sound/brick.ogg";
	string WALL_SOUND = "sound/wall.ogg";
	string CALCULATOR_SOUND = "sound/calculator.ogg";
	string GAMEOVER_SOUND = "sound/gameover.ogg";
	string MONEY_SOUND = "sound/money.ogg";
	string PLAYER_SOUND = "sound/player.ogg";
	string POWERUP_SOUND = "sound/powerup.ogg";
	string KEY_SOUND = "sound/key.ogg";
	string ALARM_SOUND = "sound/alarm.ogg";
	string DEATH_SOUND = "sound/death.ogg";
	string BACKGROUND_SOUND = "sound/background.ogg";


	static irrklang::ISoundEngine* engine;
	static irrklang::ISound* backgroundMusic;

};


#endif
