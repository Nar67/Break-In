#ifndef _BALL_INCLUDE
#define _BALL_INCLUDE


#include "Sprite.h"
#include "SpriteSheet.h"
#include "TileMap.h"
#include "Player.h"
#include "SoundManager.h"


// Player is basically a Sprite that represents the player. As such it has
// all properties it needs to track its movement, jumping, and collisions.


class Ball
{

public:
	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram);
	void update(int deltaTime);
	void render();
	void moveBall(int deltaTime);
	void setTileMap(TileMap *tileMap);
	void setPosition(const glm::vec2 &pos);
	void setPlayer(Player* play);
	void nextRoom();
	void firstRoom();
	void setStop(bool stop);
	
private:
	bool stuck, room, stop = false;
	int offsetRoom = 0;
    glm::vec2 speed;
	glm::ivec2 tileMap, posBall;
	Texture spritesheet;
	Sprite *sprite;
	TileMap *map;
	Player* player;
	SoundManager sound;

    Tile* getBallTile(glm::vec2 pos);
	bool isOverlaping(Tile*);
	bool outOfScreen(int nextPos_x, int nextPos_y);
	Tile* getTileColliding(vector<Tile*> tiles);
	bool colliding(vector<Tile*> types);
	bool collidedFromLeft(int next_x, int next_y, Tile* tile);
	bool collidedFromRight(int next_x, int next_y, Tile* tile);
	bool collidedFromBottom(int next_x, int next_y, Tile* tile);
	bool collidedFromTop(int next_x, int next_y, Tile* tile);
	bool collidedWithPlayer(int next_x, int next_y);


	void removeTile(Tile* tile);

	//debuging funcs
    void printBallTile();
	void printTile(Tile* tile);

    Tile* getBallTile(glm::vec2 pos);
	bool isOverlaping(Tile*);
	bool outOfScreen(int nextPos_x, int nextPos_y);
	Tile* getTileColliding(vector<Tile*> tiles);
	bool colliding(vector<Tile*> types);
	bool collidedFromLeft(int next_x, int next_y, Tile* tile);
	bool collidedFromRight(int next_x, int next_y, Tile* tile);
	bool collidedFromBottom(int next_x, int next_y, Tile* tile);
	bool collidedFromTop(int next_x, int next_y, Tile* tile);


	//debuging funcs
    void printBallTile();
};


#endif // _BALL_INCLUDE

