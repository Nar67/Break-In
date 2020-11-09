#ifndef TILE_H 
#define TILE_H 

#include <utility>
#include "ShaderProgram.h"
#include "Texture.h"
#include "SpriteSheet.h"
#include <vector>

using namespace std;

class Tile
{

public:
	Tile() = default;
	Tile(int x, int y, char const& tileType, ShaderProgram& shprog);
	void render();
	void init();
	void free();
	void sendVertices();
	void calculateVertices();
	SpriteType getType();
	void changeRoom();
	glm::vec2 getPosition();
	int getBlockSize();
	glm::vec2 getIndex();
	int decreaseHits();
	int getPoints();
	SpriteSheet* getSprite();

	void breakTile();

private:
	int setHits();
	int setPoints();

private:
	int xPos, yPos, room, offset, hits, points;
	vector<float> vertices;
	bool nRoom = false, pRoom = false;

	glm::vec2 posTile;
	GLuint vao, vbo;
	GLuint posLocation, texCoordLocation;
	ShaderProgram program;
	SpriteSheet* sprite;
};

#endif
