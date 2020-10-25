#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Ball.h"
#include "Game.h"


const int BALL_SIZE_X = 20;
const int BALL_SIZE_Y = 20;
const float BALL_SPEED = 0.1;

const int BALL_RADIUS = BALL_SIZE_X;

const float SCREEN_SIZE_X = 448 - BALL_SIZE_X; //448 = blockSize*mapSize.x
const float SCREEN_SIZE_Y = 476 - BALL_SIZE_Y; //476 = (blockSize/2)*mapSize.y


void Ball::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram)
{
	spritesheet.loadFromFile("images/ball_pink.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(BALL_SIZE_X, BALL_SIZE_Y), glm::vec2(1, 1), &spritesheet, &shaderProgram);
	tileMap = tileMapPos;
    speed = glm::vec2(float(BALL_SPEED), float(BALL_SPEED));
	sprite->setPosition(glm::vec2(float(posBall.x), float(posBall.y)));
}

void Ball::update(int deltaTime)
{
	sprite->update(deltaTime);

    if(!stuck)
    {
        moveBall(deltaTime);
    }

	sprite->setPosition(glm::vec2(float(posBall.x), float(posBall.y)));
}

void Ball::render()
{
	sprite->render();
}

void Ball::setTileMap(TileMap *tileMap)
{
	map = tileMap;
}

void Ball::setPosition(const glm::vec2 &pos)
{
	posBall = pos;
	sprite->setPosition(glm::vec2(float(posBall.x), float(posBall.y)));
}

void Ball::moveBall(int deltaTime)
{
    int nextPos_x = posBall.x + speed.x * deltaTime;
    SpriteType type = getBallTile(glm::vec2(nextPos_x, posBall.y))->getType();
    SpriteType typeWithOffset = getBallTile(glm::vec2(nextPos_x + BALL_SIZE_X, posBall.y))->getType();
    if(nextPos_x <= float(0))
    {
        speed.x *= -1;
        nextPos_x = 0; 
    }
    else if(nextPos_x >= SCREEN_SIZE_X)
    {
        speed.x *= -1;
        nextPos_x = SCREEN_SIZE_X; 
    }
    if(type == SpriteType::WALL or typeWithOffset == SpriteType::WALL)
    {
        speed.x *= -1;
        nextPos_x = posBall.x;
    }
    
    int nextPos_y = posBall.y + speed.y * deltaTime;
    type = getBallTile(glm::vec2(posBall.x, nextPos_y))->getType();
    typeWithOffset = getBallTile(glm::vec2(posBall.x, nextPos_y + BALL_SIZE_Y))->getType();
    if(nextPos_y <= float(0))
    {
        speed.y *= -1;
        nextPos_y = 0; 
    }
    else if(nextPos_y >= SCREEN_SIZE_Y)
    {
        speed.y *= -1;
        cout << "touching SCREEN" << endl;
        nextPos_y = SCREEN_SIZE_Y; 
    }
    if(type == SpriteType::WALL or typeWithOffset == SpriteType::WALL)
    {
        speed.y *= -1;
        cout << "touching wall Y" << endl;
        nextPos_y = posBall.y;
    }

    posBall.x = nextPos_x;
    posBall.y = nextPos_y;
    
}

Tile *Ball::getBallTile(glm::vec2 pos)
{
    int tilePos_x = pos.x / map->getBlockSize();
    int tilePos_y = pos.y / (map->getBlockSize()/2);
    return map->getTiles()[tilePos_y * map->getMapSize().x + tilePos_x];
}

void Ball::printBallTile()
{
    Tile* currentTile = getBallTile(posBall);
    SpriteType type = currentTile->getType();
	switch (type) {
	case SpriteType::KEY:
        cout << "KEY" << endl;
		break;
	case SpriteType::PLATFORM:
        cout << "PLATFORM" << endl;
		break;
    case SpriteType::WALL:
        cout << "WALL" << endl;
		break;
    case SpriteType::BLOCK:
        cout << "BLOCK" << endl;
		break;
	case SpriteType::NOTHING:
        cout << "NOTHING" << endl;
		break;
    default:
        cout << "mmmmmmmmmmmmmmmmmmmeh" << endl;
        break;
	}
}

