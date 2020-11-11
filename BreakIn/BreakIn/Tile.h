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
	void nextRoom();
	void previousRoom();
	glm::vec2 getPosition();
	int getBlockSize();
	glm::vec2 getIndex();
	int decreaseHits();
	int getPoints();
	SpriteSheet* getSprite();

	int getOffset();
	void setOffset(int off);
	int getRoom();
	void setRoom(int r);

private:
	int setHits();
	int setPoints();

private:
	int xPos, yPos, points;
	int room = 0, offset = 0;
	vector<float> vertices;
	bool nRoom = false, pRoom = false;

	glm::vec2 posTile;
	GLuint vao, vbo;
	GLuint posLocation, texCoordLocation;
	ShaderProgram program;
	SpriteSheet* sprite;
};

#endif