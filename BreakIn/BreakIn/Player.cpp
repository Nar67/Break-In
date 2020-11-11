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

#define EYE_OFFSET_X 20
#define EYE_OFFSET_Y 30

enum playerAnims {
	UP, UPRIGHT, RIGHT, DOWNRIGHT, DOWN, DOWNLEFT, LEFT, UPLEFT, IDLE, DEAD
};

Player::Player()
{
}


Player::~Player()
{
}

void Player::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram)
{
	spritesheet.loadFromFile("images/playerAnimations.png", TEXTURE_PIXEL_FORMAT_RGBA);
	tileMapDispl = tileMapPos;
	
	sprite = Sprite::createSprite(glm::ivec2(PLAYER_SIZE_X, PLAYER_SIZE_Y), glm::vec2(0.25, 0.25), &spritesheet, &shaderProgram);

	sprite->setNumberAnimations(10);
	
	sprite->setAnimationSpeed(IDLE, 8);
	sprite->addKeyframe(IDLE, glm::vec2(0.f, 0.5f));

	sprite->setAnimationSpeed(UP, 8);
	sprite->addKeyframe(UP, glm::vec2(0.f, 0.f));

	sprite->setAnimationSpeed(UPRIGHT, 8);
	sprite->addKeyframe(UPRIGHT, glm::vec2(0.25f, 0.f));

	sprite->setAnimationSpeed(RIGHT, 8);
	sprite->addKeyframe(RIGHT, glm::vec2(0.5f, 0.f));

	sprite->setAnimationSpeed(DOWNRIGHT, 8);
	sprite->addKeyframe(DOWNRIGHT, glm::vec2(0.75f, 0.f));

	sprite->setAnimationSpeed(DOWN, 8);
	sprite->addKeyframe(DOWN, glm::vec2(0.f, 0.25f));

	sprite->setAnimationSpeed(DOWNLEFT, 8);
	sprite->addKeyframe(DOWNLEFT, glm::vec2(0.25f, 0.25f));

	sprite->setAnimationSpeed(LEFT, 8);
	sprite->addKeyframe(LEFT, glm::vec2(0.5f, 0.25f));

	sprite->setAnimationSpeed(UPLEFT, 8);
	sprite->addKeyframe(UPLEFT, glm::vec2(0.75f, 0.25f));

	sprite->setAnimationSpeed(DEAD, 4);
	sprite->addKeyframe(DEAD, glm::vec2(0.25f, 0.5f));
	sprite->addKeyframe(DEAD, glm::vec2(0.5f, 0.5f));
	sprite->addKeyframe(DEAD, glm::vec2(0.75f, 0.5f));
	sprite->addKeyframe(DEAD, glm::vec2(0.f, 0.75f));
	sprite->addKeyframe(DEAD, glm::vec2(0.25f, 0.75f));

	sprite->changeAnimation(IDLE);

	sprite->setPosition(glm::vec2(float(posPlayer.x), float(posPlayer.y)));

	time = 0;
}


void Player::update(int deltaTime)
{
	sprite->update(deltaTime);
	time += deltaTime;
	if (dead) {
		dead = !dead;
		t = true;
		sprite->changeAnimation(DEAD);
	}
	if (t && time > 1100) {
		t = !t;
		setInitPos();
		sprite->changeAnimation(IDLE);
		stop = false;
	}
	if (!stop) {
		
		if (Game::instance().getSpecialKey(GLUT_KEY_LEFT))
		{
			posPlayer.x -= 3;
			if (posPlayer.x < -4) posPlayer.x += 3;
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

void Player::updateAnimation(const glm::vec2 &posBall)
{
	if(posBall.x < posPlayer.x - EYE_OFFSET_X)
	{
		if(posBall.y > posPlayer.y + EYE_OFFSET_Y) sprite->changeAnimation(DOWNLEFT);
		else if (posBall.y < posPlayer.y - EYE_OFFSET_Y) sprite->changeAnimation(UPLEFT);
		else sprite->changeAnimation(LEFT);
	}
	else if(posBall.x > posPlayer.x + EYE_OFFSET_X) 
	{
		if(posBall.y > posPlayer.y + EYE_OFFSET_Y) sprite->changeAnimation(DOWNRIGHT);
		else if(posBall.y < posPlayer.y - EYE_OFFSET_Y) sprite->changeAnimation(UPRIGHT);
		else sprite->changeAnimation(RIGHT);
	}

	else 
	{
		if(posBall.y > posPlayer.y + EYE_OFFSET_Y) sprite->changeAnimation(DOWN);
		else if(posBall.y < posPlayer.y - EYE_OFFSET_Y*2) sprite->changeAnimation(UP);
		else sprite->changeAnimation(IDLE);
	}
}

void Player::render()
{
	sprite->render();
}

void Player::setTileMap(TileMap *tileMap)
{
	if (map != NULL)
		map = NULL;
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

void Player::deadAnimation() {
	dead = true;
	time = 0;
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




