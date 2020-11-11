#include "Vigilant.h"
#include "Game.h"
#include <GL/glut.h>

#define VIGILANT_SIZE_X 28
#define VIGILANT_SIZE_Y VIGILANT_SIZE_X/2

#define MAP_OFFSET_Y 894
#define INIT_POS_X 1 * 28
#define INIT_POS_Y 17 * 28/2 + MAP_OFFSET_Y

#define EYE_OFFSET_X 20
#define EYE_OFFSET_Y 30


enum vigilantAnims {
	UP, UPRIGHT, RIGHT, DOWNRIGHT, DOWN, DOWNLEFT, LEFT, UPLEFT
};

Vigilant::Vigilant()
{
}


Vigilant::~Vigilant()
{
}


void Vigilant::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram)
{
	spritesheet.loadFromFile("images/vigilantAnimations.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(VIGILANT_SIZE_X, VIGILANT_SIZE_Y), glm::vec2(0.25f, 0.5f), &spritesheet, &shaderProgram);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(posVigilant.x), float(posVigilant.y)));

	sprite->setNumberAnimations(8);

	sprite->setAnimationSpeed(UP, 8);
	sprite->addKeyframe(UP, glm::vec2(0.f, 0.f));

	sprite->setAnimationSpeed(UPRIGHT, 8);
	sprite->addKeyframe(UPRIGHT, glm::vec2(0.25f, 0.f));

	sprite->setAnimationSpeed(RIGHT, 8);
	sprite->addKeyframe(RIGHT, glm::vec2(0.5f, 0.f));

	sprite->setAnimationSpeed(DOWNRIGHT, 8);
	sprite->addKeyframe(DOWNRIGHT, glm::vec2(0.75f, 0.f));

	sprite->setAnimationSpeed(DOWN, 8);
	sprite->addKeyframe(DOWN, glm::vec2(0.f, 0.5f));

	sprite->setAnimationSpeed(DOWNLEFT, 8);
	sprite->addKeyframe(DOWNLEFT, glm::vec2(0.25f, 0.5f));

	sprite->setAnimationSpeed(LEFT, 8);
	sprite->addKeyframe(LEFT, glm::vec2(0.5f, 0.5f));

	sprite->setAnimationSpeed(UPLEFT, 8);
	sprite->addKeyframe(UPLEFT, glm::vec2(0.75f, 0.5f));

}

void Vigilant::update(int deltaTime)
{
	sprite->update(deltaTime);

	if (!stop) {

		glm::vec2 posPlayer = player->getPos();
		if (posVigilant.x < posPlayer.x)
			posVigilant.x += 1;
		else
			posVigilant.x -= 1;
		if (posVigilant.y < posPlayer.y)
			posVigilant.y += 1;
		else
			posVigilant.y -= 1;

		if (collidedWithPlayer()) {
			sound.playGameover();
			map->deadInside();
			map->setCaught();
			player->deadAnimation();
			posVigilant.x = INIT_POS_X;
			posVigilant.y = INIT_POS_Y;
		}
		updateAnimation(posPlayer);
		sprite->setPosition(glm::vec2(float(tileMapDispl.x + posVigilant.x), float(tileMapDispl.y + posVigilant.y)));
	
	} 
}


bool Vigilant::collidedWithPlayer() {
	glm::vec2 posPlayer = player->getPos();
	int minX, maxX, maxY;
	int bbminX, bbmaxX, bbminY, bbmaxY;
	//espai que ocupa el vigilant
	minX = posVigilant.x;
	maxX = posVigilant.x + VIGILANT_SIZE_X;
	maxY = posVigilant.y + VIGILANT_SIZE_Y;
	//espai que ocupa el player
	bbminX = posPlayer.x;
	bbmaxX = posPlayer.x + 34;
	bbminY = posPlayer.y;
	bbmaxY = posPlayer.y + 46;

	return ((maxX > bbminX) && (minX < bbmaxX) &&
		(bbmaxY > maxY) && (maxY > bbminY));
}

void Vigilant::updateAnimation(const glm::vec2 &player)
{
	if(player.x < posVigilant.x - EYE_OFFSET_X)
	{
		if(player.y > posVigilant.y + EYE_OFFSET_Y) sprite->changeAnimation(DOWNLEFT);
		else if (player.y < posVigilant.y - EYE_OFFSET_Y) sprite->changeAnimation(UPLEFT);
		else sprite->changeAnimation(LEFT);
	}
	else if(player.x > posVigilant.x + EYE_OFFSET_X) 
	{
		if(player.y > posVigilant.y + EYE_OFFSET_Y) sprite->changeAnimation(DOWNRIGHT);
		else if(player.y < posVigilant.y - EYE_OFFSET_Y) sprite->changeAnimation(UPRIGHT);
		else sprite->changeAnimation(RIGHT);
	}

	else 
	{
		if(player.y > posVigilant.y + EYE_OFFSET_Y) sprite->changeAnimation(DOWN);
		else if(player.y < posVigilant.y - EYE_OFFSET_Y*2) sprite->changeAnimation(UP);
	}
}

void Vigilant::render()
{
	sprite->render();
}

void Vigilant::setTileMap(TileMap* tileMap)
{
	map = tileMap;
}

void Vigilant::setPosition(const glm::vec2& pos)
{
	posVigilant = pos;
	sprite->setPosition(glm::vec2(float(posVigilant.x), float(posVigilant.y)));
}

void Vigilant::setStop(bool stop) {
	this->stop = stop;
}

void Vigilant::setPlayer(Player* player) {
	this->player = player;
}