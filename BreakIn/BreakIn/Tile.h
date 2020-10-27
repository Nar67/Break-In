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
<<<<<<< HEAD
	void changeRoom();
	glm::vec2 getPosition();
	int getBlockSize();
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
	glm::vec2 getIndex();
	int decreaseHits();
=======
	glm::vec2 getPosition();
	int getBlockSize();
>>>>>>> redone collision detection
=======
>>>>>>> redone collision detection

private:
	int setHits();
=======
	ShaderProgram* freeAndGetProgram();
>>>>>>> tiles not removing, sadly
=======
	glm::vec2 getIndex();
>>>>>>> free tiles

private:
	int xPos, yPos, room, offset, hits;
	vector<float> vertices;
	bool nextRoom;

	glm::vec2 posTile;
	GLuint vao, vbo;
	GLuint posLocation, texCoordLocation;
	ShaderProgram program;
	SpriteSheet* sprite;
};

#endif
