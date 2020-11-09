#include "Vigilant.h"
#include "Game.h"
#include <GL/glut.h>

#define VIGILANT_SIZE_X 28
#define VIGILANT_SIZE_Y 28/2

#define MAP_OFFSET_Y 894
#define INIT_POS_X 6.5 * 28
#define INIT_POS_Y 25 * 28/2 + MAP_OFFSET_Y

Vigilant::Vigilant()
{
}


Vigilant::~Vigilant()
{
}


void Vigilant::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram)
{
	spritesheet.loadFromFile("images/Vigilant.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(VIGILANT_SIZE_X, VIGILANT_SIZE_Y), glm::vec2(1, 1), &spritesheet, &shaderProgram);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(posVigilant.x), float(posVigilant.y)));
}

void Vigilant::update(int deltaTime)
{
	sprite->update(deltaTime);

	if (!stop) {
		if (collidedWithPlayer()) {
			sound.playGameover();
			map->deadInside();
			player->setInitPos();
		}

		glm::vec2 posPlayer = player->getPos();
		if (posVigilant.x < posPlayer.x)
			posVigilant.x += 1;
		else
			posVigilant.x -= 1;
		if (posVigilant.y < posPlayer.y)
			posVigilant.y += 1;
		else
			posVigilant.y -= 1;

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