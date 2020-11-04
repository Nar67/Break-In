#ifndef _TILE_MAP_INCLUDE
#define _TILE_MAP_INCLUDE


#include <glm/glm.hpp>
#include "Texture.h"
#include <fstream>
#include <sstream>
#include <vector>
#include "ShaderProgram.h"
#include "Tile.h"


// Class Tilemap is capable of loading a tile map from a text file in a very
// simple format (see level01.txt for an example). With this information
// it builds a single VBO that contains all tiles. As a result the render
// method draws the whole map independently of what is visible.


class TileMap
{

public:
	// Tile maps can only be created inside an OpenGL context
	static TileMap *createTileMap(const string &levelFile, const glm::vec2 &minCoords, ShaderProgram &program);

	TileMap(const string &levelFile, const glm::vec2 &minCoords, ShaderProgram &program);
	~TileMap();

	void render() const;
	void free();
	void changeRoom();
	vector<Tile*> getTiles();

	glm::ivec2 getMapSize();

	int getBlockSize();

	void removeTile(Tile* tile);

	void printMap();

	void setRoom(int r);


private:
	bool loadLevel();
	void openPath();

private:
	GLuint vao;
	GLuint vbo;
	GLint posLocation, texCoordLocation;
	glm::ivec2 position, mapSize, tilesheetSize;
	int tileSize, blockSize;
	int room;
	Texture tilesheet;
	glm::vec2 tileTexSize;
	vector<Tile*> map;
	ShaderProgram program;
	string levelFile;
};


#endif // _TILE_MAP_INCLUDE


