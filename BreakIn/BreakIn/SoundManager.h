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
	void stopMenu();
	void playMenuMove();
	void playMenuSelect();

private:
	string  WIN_SOUND = "sound/win.ogg";
	string MENU_SOUND = "sound/menu.ogg";
	string MENU_MOVE = "sound/menu_move.ogg";
	string MENU_SELECT = "sound/menu_select.ogg";


	static irrklang::ISoundEngine* engine;
	static irrklang::ISound* backgroundMusic;

};


#endif
