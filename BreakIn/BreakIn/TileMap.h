#ifndef _TILE_MAP_INCLUDE
#define _TILE_MAP_INCLUDE


#include <glm/glm.hpp>
#include "Texture.h"
#include <fstream>
#include <sstream>
#include <vector>
#include "ShaderProgram.h"
#include "Tile.h"
#include "SoundManager.h"


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
	void nextRoom();
	void previousRoom();
	vector<Tile*> getTiles();
	glm::ivec2 getMapSize();
	int getBlockSize();
	void removeTile(Tile* tile);
	void printMap();
	void setRoom(int r);
	int getMoney();
	int getPoints();
	void setCalculator();
	bool getCalculator();
	void swapPoints();
	void setMoney(int money);
	void setPoints(int points);
	int getMoneyTiles();
	void deadInside();
	void setLives(int lives);
	int getLives();
	int getCurrentRoom();
	void setAlarm();
	void openPath();
	bool getAlarm();
	int getAlarmRoom();

private:
	bool loadLevel();

private:
	GLuint vao;
	GLuint vbo;
	GLint posLocation, texCoordLocation;
	glm::ivec2 position, mapSize, tilesheetSize;
	int tileSize, blockSize;
	int room, lives, points, money, moneyTiles = 0;
	Texture tilesheet;
	glm::vec2 tileTexSize;
	vector<Tile*> map;
	ShaderProgram program;
	string levelFile;
	SoundManager sound;
	bool calculator = false, alarm = false;
	int alarmRoom;
};


#endif // _TILE_MAP_INCLUDE


