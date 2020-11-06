#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Ball.h"
#include "Game.h"

#define MAP_OFFSET_Y 894

#define BALL_SIZE_X 20
#define BALL_SIZE_Y 20
#define BALL_SPEED 0.1

#define BALL_RADIUS BALL_SIZE_X

#define SCREEN_SIZE_X 448 - BALL_SIZE_X //448 = blockSize*mapSize.x
#define SCREEN_SIZE_Y 476 - BALL_SIZE_Y + MAP_OFFSET_Y //476 = (blockSize/2)*mapSize.y


void Ball::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram)
{
    spritesheet.loadFromFile("images/ball_pink.png", TEXTURE_PIXEL_FORMAT_RGBA);
    sprite = Sprite::createSprite(glm::ivec2(BALL_SIZE_X, BALL_SIZE_Y), glm::vec2(1, 1), &spritesheet, &shaderProgram);
    tileMap = tileMapPos;
    speed = glm::vec2(float(BALL_SPEED), float(BALL_SPEED));
    sprite->setPosition(glm::vec2(float(posBall.x), float(posBall.y)));
    room = false;
}

void Ball::update(int deltaTime)
{
	sprite->update(deltaTime);
    if (!stuck)
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

void Ball::setPlayer(Player *play)
{
	player = play;
}

void Ball::setPosition(const glm::vec2 &pos)
{
	posBall = pos;
	sprite->setPosition(glm::vec2(float(posBall.x), float(posBall.y)));
}

void Ball::moveBall(int deltaTime)
{
    int nextPos_x = posBall.x + speed.x * deltaTime;
    int nextPos_y = posBall.y + speed.y * -deltaTime;
    if(not outOfScreen(nextPos_x, nextPos_y)) //Ball is not out of screen
    {
        Tile *tile = getBallTile(glm::vec2(nextPos_x, nextPos_y)); //get the tile the ball is on
		Tile *tileWithOffsetX = getBallTile(glm::vec2(nextPos_x + BALL_SIZE_X, nextPos_y));
		Tile *tileWithOffsetY = getBallTile(glm::vec2(nextPos_x, nextPos_y + BALL_SIZE_Y));
		Tile *tileWithOffsetYHalf = getBallTile(glm::vec2(nextPos_x, nextPos_y + BALL_SIZE_Y/2));
        Tile *tileWithOffsetXY = getBallTile(glm::vec2(nextPos_x + BALL_SIZE_X, nextPos_y + BALL_SIZE_Y));
		Tile *tileWithOffsetXYHalf = getBallTile(glm::vec2(nextPos_x + BALL_SIZE_X, nextPos_y + BALL_SIZE_Y/2));

        vector<Tile*> tiles {tile, tileWithOffsetX, tileWithOffsetY, tileWithOffsetXY, tileWithOffsetXYHalf, tileWithOffsetYHalf};
        if(colliding(tiles)) //chech if the ball is colliding with some bounceable object
        {
            Tile *tileCollided = getTileColliding(tiles); //get the tile that is colliding with something
            SpriteType type = tileCollided->getType();
            if (type == SpriteType::BLOCK or type == SpriteType::WALL or type == SpriteType::MONEY) {
                if (collidedFromRight(nextPos_x, nextPos_y, tileCollided) or collidedFromLeft(nextPos_x, nextPos_y, tileCollided))
                {
                    speed.x *= -1;
                    nextPos_x = posBall.x;
                    nextPos_y = posBall.y;
                }
                else if (collidedFromTop(nextPos_x, nextPos_y, tileCollided) or collidedFromBottom(nextPos_x, nextPos_y, tileCollided))
                {
                    speed.y *= -1;
                    nextPos_y = posBall.y;
                    nextPos_x = posBall.x;
                }
                switch (type) {
                    case SpriteType::WALL:
                        sound.playWall();
                        break;
                    case SpriteType::MONEY:
                        sound.playMoney();
                        break;
                    default:
                        sound.playBrick();
                        break;
                }
            }
            if (type == SpriteType::ARROW ) {
                if (nextPos_y < posBall.y) {
                    if (room) {
                        map->nextRoom();
                        room = !room;
                        nextPos_y += 400;
                        offsetRoom += 32;
                    }
                }
            }
            if (type == SpriteType::KEY) {
                room = true;
            }
            removeTile(tileCollided);
        }
        if(collidedWithPlayer(nextPos_x, nextPos_y))
        {
            speed.y *= -1;
            nextPos_y = posBall.y;
            nextPos_x = posBall.x;
            sound.playPlayer();
        }
        posBall.x = nextPos_x;
        posBall.y = nextPos_y;
    }

}

Tile *Ball::getBallTile(glm::vec2 pos)
{
    int tilePos_x = pos.x / map->getBlockSize();
    int tilePos_y = pos.y / (map->getBlockSize()/2) - offsetRoom;
    return map->getTiles()[tilePos_y * map->getMapSize().x + tilePos_x];
}

Tile *Ball::getTileColliding(vector<Tile*> tiles)
{
	for(auto tile : tiles)
	{
        SpriteType st = tile->getType();
        if (st == SpriteType::WALL or st == SpriteType::BLOCK
            or st == SpriteType::KEY or st == SpriteType::ARROW
            or st == SpriteType::MONEY)
    	{
        	return tile;
    	}
	}
    
}

bool Ball::colliding(vector<Tile*> tiles)
{
    for(auto tile : tiles)
	{
        SpriteType st = tile->getType();
		if(st == SpriteType::WALL or st == SpriteType::BLOCK
            or st == SpriteType::KEY or st == SpriteType::ARROW
            or st == SpriteType::MONEY)
    	{
        	return true;
    	}
	}
	return false;
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

bool Ball::collidedWithPlayer(int next_x, int next_y)
{
    return posBall.y + BALL_SIZE_Y < player->getPosition().y &&
            next_y + BALL_SIZE_Y >= player->getPosition().y &&
            ((next_x > player->getPosition().x && next_x < player->getPosition().x + player->getPlayerXSize() 
            or next_x + BALL_SIZE_X > player->getPosition().x && next_x + BALL_SIZE_X < player->getPosition().x + player->getPlayerXSize()));
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
    switch (currentTile->getType()) {
	case SpriteType::KEY:
        cout << "KEY" << endl;
		break;
    case SpriteType::WALL:
        cout << "WALL" << endl;
		break;
    case SpriteType::BLOCK:
        cout << "BLOCK" << endl;
		break;
    case SpriteType::MONEY:
        cout << "MONEY" << endl;
		break;
    case SpriteType::ALARM:
        cout << "ALARM" << endl;
		break;
	case SpriteType::NOTHING:
        cout << "NOTHING" << endl;
		break;
    default:
        cout << "mmmmmmmmmmmmmmmmmmmeh" << endl;
        break;
	}
}

void Ball::printTile(Tile* tile)
{
	switch (tile->getType()) {
	case SpriteType::KEY:
        cout << "KEY" << endl;
		break;
    case SpriteType::WALL:
        cout << "WALL" << endl;
		break;
    case SpriteType::BLOCK:
        cout << "BLOCK" << endl;
		break;
    case SpriteType::MONEY:
        cout << "MONEY" << endl;
		break;
    case SpriteType::ALARM:
        cout << "ALARM" << endl;
		break;
	case SpriteType::NOTHING:
        cout << "NOTHING" << endl;
		break;
    default:
        cout << "mmmmmmmmmmmmmmmmmmmeh" << endl;
        break;
	}
}

void Ball::removeTile(Tile* tile)
{
    map->removeTile(tile);
}

void Ball::nextRoom() {
    offsetRoom+=32;
}

void Ball::firstRoom() {
    offsetRoom = 0;
}