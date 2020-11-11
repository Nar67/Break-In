#include <GL/glew.h>
#include <GL/glut.h>
#include "Game.h"


void Game::init()
{
	bPlay = true;
	glClearColor(0.f, 0.f, 0.f, 1.0f);
	state = GameState::MENU;
	sound.init();
	menu.init();
	scene.init();
}

bool Game::update(int deltaTime)
{
	switch (state)
	{
	case GameState::MENU:
		menu.update(deltaTime);
		break;
	case GameState::GAME:
		scene.update(deltaTime);
		break;
	case GameState::CREDITS:
		credits.update(deltaTime);
		break;
	case GameState::INSTRUCTIONS:
		instructions.update(deltaTime);
		break;
	case GameState::SHORTCUTS:
		shortcuts.update(deltaTime);
		break;
	default:
		break;
	}

	return bPlay;
}

void Game::render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//scene.render();
	switch (state)
	{
	case GameState::MENU:
		menu.render();
		break;
	case GameState::GAME:
		
		scene.render();
		break;
	case GameState::CREDITS:
		credits.render();
		break;
	case GameState::INSTRUCTIONS:
		instructions.render();
		break;
	case GameState::SHORTCUTS:
		shortcuts.render();
		break;
	default:
		break;
	}
}

void Game::keyPressed(int key)
{
	if (key == 27) // Escape code
		if (state == GameState::MENU) {
			bPlay = false;
		}
		else {
			state = GameState::MENU;
			if (!sound.isCurrentlyPlaying("sound/menu.ogg"))
				sound.playMenu();
			if (sound.isCurrentlyPlaying("sound/alarm.ogg"))
				sound.stopSound("sound/alarm.ogg");
			if (sound.isCurrentlyPlaying("sound/background.ogg"))
				sound.stopBackground();
		}
			

	if (key == 'f' || key == 'F') {
		fullscreen = !fullscreen;
		if (fullscreen)
			glutFullScreen();
		else {
			glutPositionWindow(100, 100);
			glutReshapeWindow(640, 480);
		}
	}

	if (key == 'n' || key == 'N') {
		if (state == GameState::GAME) {
			scene.nextRoom();
		}
	}

	if (key == 'b' || key == 'B') {
		if (state == GameState::GAME) {
			scene.nextBank();
		}
	}

	if (key == 'r' || key == 'R') {
		if (state == GameState::GAME) {
			scene.restart();
		}
	}



	keys[key] = true;
}

void Game::keyReleased(int key)
{
	keys[key] = false;
}

void Game::specialKeyPressed(int key)
{
	specialKeys[key] = true;
}

void Game::specialKeyReleased(int key)
{
	specialKeys[key] = false;
}

void Game::mouseMove(int x, int y)
{
}

void Game::mousePress(int button)
{
}

void Game::mouseRelease(int button)
{
}

bool Game::getKey(int key) const
{
	return keys[key];
}

bool Game::getSpecialKey(int key) const
{
	return specialKeys[key];
}

void Game::changeMode(int mode) {
	switch (mode)
	{
	case 0:
		state = GameState::GAME;
		sound.stopSound("sound/menu.ogg");
		scene.restart();
		scene.init();
		sound.playBackground();
		break;
	case 1:
		state = GameState::INSTRUCTIONS;
		instructions.init();
		break;
	case 2:
		state = GameState::SHORTCUTS;
		shortcuts.init();
		break;
	case 3:
		state = GameState::CREDITS;
		credits.init();
		break;
	
	default:
		break;
	}
}




