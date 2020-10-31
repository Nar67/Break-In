#ifndef _GAME_INCLUDE
#define _GAME_INCLUDE


#include "Scene.h"
#include "Menu.h"
#include "Instructions.h"
#include "Credits.h"
#include "ShortCuts.h"
#include "SoundManager.h"


// Game is a singleton (a class with a single instance) that represents our whole application

enum class GameState { MENU, GAME, CREDITS, INSTRUCTIONS, SHORTCUTS};

class Game
{

public:
	Game() {}
	
	
	static Game &instance()
	{
		static Game G;
	
		return G;
	}
	
	void init();
	bool update(int deltaTime);
	void render();
	
	// Input callback methods
	void keyPressed(int key);
	void keyReleased(int key);
	void specialKeyPressed(int key);
	void specialKeyReleased(int key);
	void mouseMove(int x, int y);
	void mousePress(int button);
	void mouseRelease(int button);
	void changeMode(int mode);
	
	bool getKey(int key) const;
	bool getSpecialKey(int key) const;

private:
	bool bPlay, fullscreen = false;                       // Continue to play game?
	Scene scene;                      // Scene to render
	bool keys[256], specialKeys[256]; // Store key states so that 
	                                  // we can have access at any time
	GameState state;
	Menu menu;
	Instructions instructions;
	Credits credits;
	ShortCuts shortcuts;
	SoundManager sound;
};


#endif // _GAME_INCLUDE


