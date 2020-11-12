#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Ball.h"
#include "Game.h"

#define MAP_OFFSET_Y 894
#define INIT_POS_X 8 * 28
#define INIT_POS_Y 25 * 28/2 + MAP_OFFSET_Y

#define BALL_SIZE_X 20
#define BALL_SIZE_Y 20
#define BALL_SPEED 0.2

#define BALL_RADIUS BALL_SIZE_X

#define SCREEN_SIZE_X 448 - BALL_SIZE_X //448 = blockSize*mapSize.x
#define SCREEN_SIZE_Y 476 - BALL_SIZE_Y + MAP_OFFSET_Y //476 = (blockSize/2)*mapSize.y


#define BALL_RADIUS BALL_SIZE_X/2

#define SCREEN_SIZE_X 448 - BALL_SIZE_X //448 = blockSize*mapSize.x
#define SCREEN_SIZE_Y 476 - BALL_SIZE_Y + MAP_OFFSET_Y //476 = (blockSize/2)*mapSize.y


void Ball::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram)
{
    spritesheet.loadFromFile("images/ball_pink.png", TEXTURE_PIXEL_FORMAT_RGBA);
    sprite = Sprite::createSprite(glm::ivec2(BALL_SIZE_X, BALL_SIZE_Y), glm::vec2(1, 1), &spritesheet, &shaderProgram);
    tileMap = tileMapPos;
    speed = glm::vec2(float(BALL_SPEED), float(BALL_SPEED));
    setPosition(glm::vec2((player->getPosition().x + player->getPlayerXSize()/2) - BALL_RADIUS, player->getPosition().y - BALL_SIZE_Y));  
    stuck = true;
}

void Ball::update(int deltaTime)
{
	sprite->update(deltaTime);

    if (!stop) {
        if (!stuck)
        {
            moveBall(deltaTime);
            player->updateAnimation(posBall);
        }
        else {
            if (Game::instance().getSpecialKey(GLUT_KEY_LEFT) || Game::instance().getSpecialKey(GLUT_KEY_RIGHT) ||
                Game::instance().getSpecialKey(GLUT_KEY_UP) || Game::instance().getSpecialKey(GLUT_KEY_DOWN)) {

                stuck = false;
            }
        }

        sprite->setPosition(glm::vec2(float(posBall.x), float(posBall.y)));
    } 
    if (player->getPlay()) {
        stop = false;
        posBall.x = INIT_POS_X;
        posBall.y = INIT_POS_Y;
        stuck = true;
        speed = glm::vec2(float(BALL_SPEED), float(BALL_SPEED));
    }
}

void Ball::render()
{
	sprite->render();
}

void Ball::setTileMap(TileMap *tileMap)
{
    if (map != NULL)
    {
        map = NULL;
    }
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
    if(Game::instance().getKey('g') or Game::instance().getKey('G'))
    {
        godMode = !godMode;
    }
    playerCollisionCooldown += deltaTime;
    int nextPos_x = posBall.x + speed.x * deltaTime;
    int nextPos_y = posBall.y + speed.y * -deltaTime;
    if(not outOfScreen(nextPos_x, nextPos_y)) //Ball is not out of screen
    {
        if(playerCollisionCooldown > 200 && !arrow && collidedWithPlayer(nextPos_x, nextPos_y))
        {
            playerCollisionCooldown = 0;
            float centerPlayer = player->getPosition().x + player->getPlayerXSize()/2;
            float dist = (posBall.x + BALL_RADIUS) - centerPlayer;
            float percent = dist / (player->getPlayerXSize()/2);
            float strength = 1.0f;
            if(godMode)
            {
                strength = 0.f;
            }
            glm::vec2 oldSpeed = speed;
            speed.x = BALL_SPEED * percent * strength;
            speed.y *= -1;
            speed = normalize(speed) * glm::length(oldSpeed);
            nextPos_y = posBall.y + speed.y;
            nextPos_x = posBall.x + speed.x;

            sound.playPlayer();
        }
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
            if (type == SpriteType::BLOCK or type == SpriteType::WALL 
                or type == SpriteType::MONEY or type == SpriteType::KEY
                or type == SpriteType::CALCULATOR or type == SpriteType::ALARM) {
                if (collidedFromRight(nextPos_x, nextPos_y, tileCollided))
                {
                    speed.x *= -1;
                    nextPos_x = posBall.x + (posBall.x - (tileCollided->getPosition().x + tileCollided->getBlockSize()));
                    nextPos_y = posBall.y;
                }
                else if(collidedFromLeft(nextPos_x, nextPos_y, tileCollided))
                {
                    speed.x *= -1;
                    nextPos_x = posBall.x - ((posBall.x + BALL_SIZE_X)- tileCollided->getPosition().x);
                    nextPos_y = posBall.y;
                }
                else if (collidedFromTop(nextPos_x, nextPos_y, tileCollided))
                {
                    speed.y *= -1;
                    nextPos_y = posBall.y - ((posBall.y + BALL_SIZE_Y) - tileCollided->getPosition().y);
                    nextPos_x = posBall.x;
                }
                else if(collidedFromBottom(nextPos_x, nextPos_y, tileCollided)) 
                {
                    speed.y *= -1;
                    nextPos_y = posBall.y + (posBall.y - (tileCollided->getPosition().y + tileCollided->getBlockSize()/2));
                    nextPos_x = posBall.x;
                }
                switch (type) {
                    case SpriteType::WALL:
                        sound.playWall();
                        break;
                    case SpriteType::MONEY:
                        sound.playMoney();
                        break;
                    case SpriteType::ALARM:
                        map->setAlarm(true);
                        break;
                    case SpriteType::KEY:
                        sound.playKey();
                        break;
                    case SpriteType::CALCULATOR:
                        sound.playCalculator();
                        stop = true;
                        player->setStop(stop);
                        break;
                    default:
                        sound.playBrick();
                        break;
                }
                arrow = false;
            }
            if (type == SpriteType::ARROW ) {
                arrow = true;
                if (nextPos_y < posBall.y) {
                    map->nextRoom();
                    offsetRoom += 32;
                    nextPos_y += 400;
                }
                else {
                    map->previousRoom();
                    nextPos_y -= 400;
                    offsetRoom -= 32;
                }
            }
            if (type == SpriteType::DEATH) {
                stuck = true;
                sound.playGameover();
                map->deadInside();
                setStop(true);
                nextPos_x = INIT_POS_X;
                nextPos_y = INIT_POS_Y;
                speed = glm::vec2(float(BALL_SPEED), float(BALL_SPEED));
                player->deadAnimation();
            }
            removeTile(tileCollided);
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
            or st == SpriteType::MONEY or st == SpriteType::DEATH
            or st == SpriteType::CALCULATOR or st == SpriteType::ALARM)
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
            or st == SpriteType::MONEY or st == SpriteType::DEATH
            or st == SpriteType::CALCULATOR or st == SpriteType::ALARM)
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
    glm::vec2 posPlayer = player->getPosition();
    if(speed.y < 0.0f && (posBall.y + BALL_SIZE_Y < posPlayer.y or posBall.y + BALL_SIZE_Y < posPlayer.y + 10))
    {
        if(next_y + BALL_SIZE_Y >= posPlayer.y &&
                ((next_x > posPlayer.x && next_x < posPlayer.x + player->getPlayerXSize())
                or (next_x + BALL_SIZE_X > posPlayer.x && next_x + BALL_SIZE_X < posPlayer.x + player->getPlayerXSize())))
        {
            posBall.y -= ((posBall.y + BALL_SIZE_Y) - posPlayer.y);
            return true;
        }
    }
    return false;
     
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

void Ball::setStop(bool stop) {
    this->stop = stop;
    player->setStop(stop);
}
