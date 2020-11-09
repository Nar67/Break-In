#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Player.h"
#include "Game.h"

#define PLAYER_SIZE_X 34
#define PLAYER_SIZE_Y 46

#define MAP_OFFSET_Y 894
#define INIT_POS_X 6.5 * 28
#define INIT_POS_Y 25 * 28/2 + MAP_OFFSET_Y

Player::Player()
{
}


Player::~Player()
{
}

void Player::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram)
{
	spritesheet.loadFromFile("images/player.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(PLAYER_SIZE_X, PLAYER_SIZE_Y), glm::vec2(1, 1), &spritesheet, &shaderProgram);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(posPlayer.x), float(posPlayer.y)));
}


void Player::update(int deltaTime)
{
	sprite->update(deltaTime);
	if (!stop) {

		if (Game::instance().getSpecialKey(GLUT_KEY_LEFT))
		{
			posPlayer.x -= 3;
			if (posPlayer.x < -4)
				posPlayer.x += 3;

		}
		else if (Game::instance().getSpecialKey(GLUT_KEY_RIGHT))
		{
			posPlayer.x += 3;
			if (posPlayer.x > 13 * 28 - 10)
				posPlayer.x -= 3;
		}

		if (Game::instance().getSpecialKey(GLUT_KEY_UP))
		{
			posPlayer.y -= 3;
			if (posPlayer.y < 16 + 894)
				posPlayer.y += 3;

		}
		else if (Game::instance().getSpecialKey(GLUT_KEY_DOWN))
		{
			posPlayer.y += 3;
			if (posPlayer.y > 27 * 16 - 14 + 894)
				posPlayer.y -= 3;
		}

		sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
	}
}

void Player::render()
{
	sprite->render();
}

void Player::setTileMap(TileMap *tileMap)
{
	map = tileMap;
}

void Player::setPosition(const glm::vec2 &pos)
{
	posPlayer = pos;
	sprite->setPosition(glm::vec2(float(posPlayer.x), float(posPlayer.y)));
}

void Player::setInitPos() {
	posPlayer.x = INIT_POS_X;
	posPlayer.y = INIT_POS_Y;
}


glm::ivec2 Player::getPosition()
{
	return posPlayer + tileMapDispl;
}
glm::ivec2 Player::getPos() {
	return posPlayer;
}

int Player::getPlayerXSize()
{
	return PLAYER_SIZE_X;
}

void Player::setStop(bool stop) {
	this->stop = stop;
}




