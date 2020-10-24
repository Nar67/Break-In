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

private:
	int xPos, yPos;
	vector<float> vertices;

	GLuint vao, vbo;
	GLuint posLocation, texCoordLocation;
	ShaderProgram program;
	SpriteSheet sprite;
};

#endif
