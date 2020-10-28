#ifndef _BALL_INCLUDE
#define _BALL_INCLUDE


#include "Sprite.h"
#include "SpriteSheet.h"
#include "TileMap.h"


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
	
private:
    bool stuck;
    glm::vec2 speed;
	glm::ivec2 tileMap, posBall;
	int jumpAngle, startY;
	Texture spritesheet;
	Sprite *sprite;
	TileMap *map;

    Tile* getBallTile(glm::vec2 pos);
	bool isOverlaping(Tile*);
	bool outOfScreen(int nextPos_x, int nextPos_y);
	Tile* getTileColliding(vector<Tile*> tiles);
	bool colliding(vector<Tile*> types);
	bool collidedFromLeft(int next_x, int next_y, Tile* tile);
	bool collidedFromRight(int next_x, int next_y, Tile* tile);
	bool collidedFromBottom(int next_x, int next_y, Tile* tile);
	bool collidedFromTop(int next_x, int next_y, Tile* tile);


	void removeTile(Tile* tile);

	//debuging funcs
    void printBallTile();
	string printTile(Tile* tile);
    void printBallTile();
	bool isOverlaping(Tile*);
	bool outOfScreen(int nextPos_x, int nextPos_y);
	Tile* getTileColliding(Tile* tile1, Tile* tile2, Tile* tile3, Tile* tile4);
	bool colliding(SpriteType tile1, SpriteType tile2, SpriteType tile3, SpriteType tile4);
	bool collidedFromLeft(int next_x, int next_y, Tile* tile);
	bool collidedFromRight(int next_x, int next_y, Tile* tile);
	bool collidedFromBottom(int next_x, int next_y, Tile* tile);
	bool collidedFromTop(int next_x, int next_y, Tile* tile);


	//debuging funcs
    void printBallTile();
	string printTile(Tile* tile);

};


#endif // _BALL_INCLUDE

