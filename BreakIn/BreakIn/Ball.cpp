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
    int nextPos_y = posBall.y + speed.y * deltaTime;
    if(not outOfScreen(nextPos_x, nextPos_y)) //Ball is not out of screen
    {
        Tile *tile = getBallTile(glm::vec2(nextPos_x, nextPos_y)); //get the tile the ball is on
		Tile *tileWithOffsetX = getBallTile(glm::vec2(nextPos_x + BALL_SIZE_X, nextPos_y));
		Tile *tileWithOffsetY = getBallTile(glm::vec2(nextPos_x, nextPos_y + BALL_SIZE_Y));
        Tile *tileWithOffsetXY = getBallTile(glm::vec2(nextPos_x + BALL_SIZE_X, nextPos_y + BALL_SIZE_Y));
        if(colliding(tile->getType(), tileWithOffsetX->getType(), tileWithOffsetY->getType(), tileWithOffsetXY->getType())) //chech if the ball is colliding with some bounceable object
        {
            Tile *tileCollided = getTileColliding(tile, tileWithOffsetX, tileWithOffsetY, tileWithOffsetXY); //get the tile that is colliding with something
            if(collidedFromTop(nextPos_x, nextPos_y, tileCollided) or collidedFromBottom(nextPos_x, nextPos_y, tileCollided))
            {
				cout << "FROM TOP OR BOTTOM, TYPE-> " << printTile(tileCollided) << endl;
                speed.y *= -1;
                nextPos_y = posBall.y;
                nextPos_x = posBall.x;
            }
            else if(collidedFromRight(nextPos_x, nextPos_y, tileCollided) or collidedFromLeft(nextPos_x, nextPos_y, tileCollided))
            {
				cout << "FROM RIGHT OR LEFT, TYPE-> " << printTile(tileCollided) << endl;
                speed.x *= -1;
                nextPos_x = posBall.x;
                nextPos_y = posBall.y;
            }
			cout << "TILE:          x: " << tileCollided->getPosition().x << "\ty: " << tileCollided->getPosition().y << endl; 
			cout << "BALL:          x: " << posBall.x << "\ty: " << posBall.y << endl << endl << endl;

			
        }
        posBall.x = nextPos_x;
        posBall.y = nextPos_y;
    }

}

Tile *Ball::getBallTile(glm::vec2 pos)
{
    int tilePos_x = pos.x / map->getBlockSize();
    int tilePos_y = pos.y / (map->getBlockSize()/2);
    return map->getTiles()[tilePos_y * map->getMapSize().x + tilePos_x];
}

Tile *Ball::getTileColliding(Tile* tile1, Tile* tile2, Tile* tile3, Tile* tile4) //TODO check if two collisions can happen at the same frame
{
    if(tile1->getType() == SpriteType::WALL or tile1->getType() == SpriteType::BLOCK) //or tile1->getType() == SpriteType::PLATFORM)
    {
        return tile1;
    }
    else if(tile2->getType() == SpriteType::WALL or tile2->getType() == SpriteType::BLOCK) //or tile2->getType() == SpriteType::PLATFORM)
    {
        return tile2;
    }
	else if(tile3->getType() == SpriteType::WALL or tile3->getType() == SpriteType::BLOCK) //or tile3->getType() == SpriteType::PLATFORM)
    {
        return tile3;
    }
	else
	{
		return tile4;
	}
	
}

bool Ball::colliding(SpriteType tile1, SpriteType tile2, SpriteType tile3, SpriteType tile4)
{
    return tile1 == SpriteType::WALL or tile1 == SpriteType::BLOCK or //or tile1 == SpriteType::PLATFORM) 
        tile2 == SpriteType::WALL or tile2 == SpriteType::BLOCK or //or tile2 == SpriteType::PLATFORM
		tile3 == SpriteType::WALL or tile3 == SpriteType::BLOCK or //or tile3 == SpriteType::PLATFORM)
		tile4 == SpriteType::WALL or tile4 == SpriteType::BLOCK; //or tile4 == SpriteType::PLATFORM)
}

bool Ball::collidedFromLeft(int next_x, int next_y, Tile* tile)
{
    return posBall.x + BALL_SIZE_X < tile->getPosition().x &&
            next_x + BALL_SIZE_X >= tile->getPosition().x;
}

bool Ball::collidedFromRight(int next_x, int next_y, Tile* tile)
{
    return posBall.x >= tile->getPosition().x + tile->getBlockSize() &&
            next_x < tile->getPosition().x + tile->getBlockSize();
}

bool Ball::collidedFromBottom(int next_x, int next_y, Tile* tile)
{
    return posBall.y >= tile->getPosition().y + tile->getBlockSize()/2 &&
            next_y < tile->getPosition().y + tile->getBlockSize()/2;
}

bool Ball::collidedFromTop(int next_x, int next_y, Tile* tile)
{
    return posBall.y + BALL_SIZE_Y < tile->getPosition().y &&
            next_y + BALL_SIZE_Y >= tile->getPosition().y;
}

bool Ball::outOfScreen(int nextPos_x, int nextPos_y)
{
    if(nextPos_x <= float(0))
    {
        speed.x *= -1;
        posBall.x = 1;
        return true;
    }
    else if(nextPos_x >= SCREEN_SIZE_X)
    {
        speed.x *= -1;
        posBall.x = SCREEN_SIZE_X-1; 
        return true;
    }
    if(nextPos_y <= float(0))
    {
        speed.y *= -1;
        posBall.y = 1;
        return true;
    }
    else if(nextPos_y >= SCREEN_SIZE_Y)
    {
        speed.y *= -1;
        posBall.y = SCREEN_SIZE_Y-1;
        return true;
    }
    return false;
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

string Ball::printTile(Tile* tile)
{
    SpriteType type = tile->getType();
	switch (type) {
	case SpriteType::KEY:
        return "KEY";
		break;
	case SpriteType::PLATFORM:
        return "PLATFORM";
		break;
    case SpriteType::WALL:
        return "WALL";
		break;
    case SpriteType::BLOCK:
        return "BLOCK";
		break;
	case SpriteType::NOTHING:
        return "NOTHING";
		break;
    default:
        return "mmmmmmmmmmmmmmmmmmmeh";
        break;
	}
}

