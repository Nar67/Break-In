#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Ball.h"
#include "Game.h"


const int BALL_SIZE_X = 20;
const int BALL_SIZE_Y = 20;



void Ball::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram)
{
	spritesheet.loadFromFile("images/ball_pink.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(BALL_SIZE_X, BALL_SIZE_Y), glm::vec2(1, 1), &spritesheet, &shaderProgram);
	tileMapDispl = tileMapPos;
    speed = glm::vec2(float(0.1), float(0.1));
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posBall.x), float(tileMapDispl.y + posBall.y)));
}

void Ball::update(int deltaTime)
{
	sprite->update(deltaTime);

    cout << "x: " << posBall.x << " y: " << posBall.y << endl;

    moveBall(deltaTime);

	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posBall.x), float(tileMapDispl.y + posBall.y)));
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
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posBall.x), float(tileMapDispl.y + posBall.y)));
}

void Ball::moveBall(int deltaTime)
{
    if(!stuck)
    {
        posBall.x += speed.x * deltaTime;
        posBall.y += speed.y * deltaTime;
        if( posBall.x <= float(0))
        {
            speed.x *= -1;
            posBall.x = 0; 
        }
        else if(posBall.x >= float(460))
        {
            speed.x *= -1;
            posBall.x = 460; 
        }
        if(posBall.y <= float(0))
        {
            speed.y *= -1;
            posBall.y = 0; 
        }
        else if(posBall.y >= float(460))
        {
            speed.y *= -1;
            posBall.y = 460; 
        }
    }
    
}

